#include <QSpinBox>

#include "dc.hh"

#include "main_window.h"
#include "ui_main_window.h"
#include "poly_2D_widget.h"
#include "poly_3D_widget.h"
#include "slicer_module.h"
#include "chmutov.hpp"
#include "gyroid.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include "periodic_columns.hpp"
#include "support.hpp"
#include "cylinder_section.hpp"
#include "rbf_surface.hpp"
#include "gcode_writer.h"
#include "fdm_obj.hpp"

#include "ipia.hh"
#include "implicit-curvatures.hh"

QProgressBar *main_window::slice_bar = nullptr;

main_window::main_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), g_writer("result.txt"), printable(sliced_obj)
{
    ui->setupUi(this);

    QObject::connect(ui->verticalScrollBar, &QScrollBar::valueChanged, ui->widget, &poly_2D_widget::slot_layer_changed);
    QObject::connect(ui->verticalScrollBar, &QScrollBar::valueChanged, ui->widget_2, &poly_3D_widget::slot_layer_changed);

    QObject::connect(ui->surface_loader_button, &QPushButton::pressed, this, &main_window::load_object);
    QObject::connect(ui->slice_button, &QPushButton::pressed, this, &main_window::slice_object);

    QObject::connect(ui->layer_count_box, qOverload<int>(&QSpinBox::valueChanged), this, &main_window::set_level_count);

    QObject::connect(ui->inner_shell_count_box, qOverload<int>(&QSpinBox::valueChanged), this, &main_window::set_inner_shell_count);

    QObject::connect(ui->inner_shell_distance_box, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &main_window::set_inner_shell_distance);

    QObject::connect(ui->infill_number_box, qOverload<int>(&QSpinBox::valueChanged), this, &main_window::set_infill_number_rot);

    QObject::connect(ui->infill_space_between_box, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &main_window::set_infill_space_between);

    QObject::connect(ui->support_checkbox, qOverload<int>(&QCheckBox::stateChanged), this, &main_window::show_support);

    QObject::connect(ui->support_gen_button, &QPushButton::pressed, this, &main_window::generate_support);

    QObject::connect(ui->zoom_scrollbar, &QScrollBar::valueChanged, ui->widget_2, &poly_3D_widget::zoom_camera);
    ui->zoom_scrollbar->valueChanged(0);

    main_window::slice_bar = ui->slice_bar;
    ui->slice_bar->setValue(0);
    ui->surface_selector_box->addItem("Chmutov", QVariant(surface_type::CHMUTOV));
    ui->surface_selector_box->addItem("Gyroid", QVariant(surface_type::GYROID));
    ui->surface_selector_box->addItem("Sphere", QVariant(surface_type::SPHERE));
    ui->surface_selector_box->addItem("Cylinder", QVariant(surface_type::CYLINDER));

    QObject::connect(ui->surface_selector_box, qOverload<int>(&QComboBox::currentIndexChanged), this, &main_window::set_surface_type);

    set_values_from_settings();

    this->setStyleSheet("QMainWindow { background-color: grey; }");
    ui->verticalScrollBar->setStyleSheet("QScrollBar:vertical { background-color: red; }");

    ui->widget->set_obj(&printable);
    ui->widget_2->set_obj(&printable);
}

main_window::~main_window()
{
    delete ui;
}

void main_window::set_values_from_settings()
{
    ui->layer_count_box->setValue(settings::level_count);
    ui->inner_shell_count_box->setValue(settings::inner_shell_count);
    ui->inner_shell_distance_box->setValue(settings::inner_shell_distance);
    ui->infill_number_box->setValue(settings::infill_number_rot);
    ui->infill_space_between_box->setValue(settings::infill_space_between);
    ui->support_space_between_box->setValue(settings::support_space_between);
}

void main_window::update_sliced_views()
{
    ui->widget->update();
    ui->widget_2->update();
}

std::array<Geometry::Point3D, 2> boundingBox(const Geometry::TriMesh &mesh)
{
    Geometry::Point3D boxmin, boxmax;
    const auto &points = mesh.points();
    boxmin = boxmax = points[0];
    for (const auto &p : points)
        for (int i = 0; i < 3; ++i)
        {
            boxmin[i] = std::min(boxmin[i], p[i]);
            boxmax[i] = std::max(boxmax[i], p[i]);
        }
    // Add 5%
    auto mean = (boxmin + boxmax) / 2;
    boxmin = mean + (boxmin - mean) * 1.05;
    boxmax = mean + (boxmax - mean) * 1.05;
    return {boxmin, boxmax};
}

std::array<size_t, 3> computeResolution(const std::array<Geometry::Point3D, 2> &bbox, size_t size)
{
    std::array<size_t, 3> resolution;
    auto axis = bbox[1] - bbox[0];
    double axis_delta = axis.norm() / size / std::sqrt(3);
    resolution[0] = std::max<size_t>((size_t)std::ceil(axis[0] / axis_delta), 2);
    resolution[1] = std::max<size_t>((size_t)std::ceil(axis[1] / axis_delta), 2);
    resolution[2] = std::max<size_t>((size_t)std::ceil(axis[2] / axis_delta), 2);
    return resolution;
}

void approximateNormals(std::vector<IPIA::PointNormal> &pns,
                        const std::list<Geometry::TriMesh::Triangle> &tris)
{
    // Weights according to:
    //   N. Max, Weights for computing vertex normals from facet normals.
    //     Journal of Graphics Tools, Vol. 4(2), 1999.
    for (const auto &tri : tris)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            size_t i0 = tri[i], i1 = tri[(i + 1) % 3], i2 = tri[(i + 2) % 3];
            Geometry::Vector3D v1 = pns[i0].p - pns[i2].p, v2 = pns[i1].p - pns[i0].p;
            double w = v1.normSqr() * v2.normSqr();
            pns[i0].n += (v1 ^ v2) / (w == 0.0 ? 1.0 : w);
        }
    }
    for (auto &pn : pns)
        if (pn.n.norm() > Geometry::epsilon)
            pn.n.normalize();
}

void main_window::load_object()
{

    auto mesh = Geometry::TriMesh::readOBJ("stanford-bunny.obj");

    auto bbox = boundingBox(mesh);
    size_t control = 50;
    auto size = computeResolution(bbox, control);
    size_t res = 20;
    auto dc_res = computeResolution(bbox, res);
    IPIA surface(3, bbox, size);

    std::vector<IPIA::PointNormal> samples;
    size_t n = mesh.points().size();
    samples.resize(n);
    for (size_t i = 0; i < n; ++i)
        samples[i] = {mesh[i], {0, 0, 0}};

    approximateNormals(samples, mesh.triangles());

    double step = 0.001;
    size_t iterations = 20;
    double tol = 0.00001;
    size_t done_iterations = surface.fit(samples, step, iterations, tol);

    auto eval = [&](const DualContouring::Point3D &p)
    { return surface({p[0] / 10, p[1] / 10, p[2] / 10}); };
    std::array<DualContouring::Point3D, 2> dc_bbox = {{{bbox[0][0] * 10, bbox[0][1] * 10, bbox[0][2] * 10},
                                                       {bbox[1][0] * 10, bbox[1][1] * 10, bbox[1][2] * 10}}};
    auto dc_mesh = DualContouring::isosurface(eval, 0.0, dc_bbox, dc_res);

    std::shared_ptr<DualContouring::QuadMesh> qm = std::make_shared<DualContouring::QuadMesh>(dc_mesh);
    ui->widget_3->set_obj(qm);

    switch (settings::s_type)
    {
    case surface_type::CHMUTOV:
        cutable_obj = std::make_shared<chmutov>();
        break;

    case surface_type::GYROID:
        cutable_obj = std::make_shared<gyroid>();
        break;
    case surface_type::SPHERE:
        cutable_obj = std::make_shared<sphere>();
        break;
    case surface_type::CYLINDER:
        cutable_obj = std::make_shared<cylinder>();
        break;
    default:
        break;
    }
    /*
        std::shared_ptr<DualContouring::QuadMesh> qm = std::make_shared<DualContouring::QuadMesh>(DualContouring::isosurface([this](const DualContouring::Point3D &p)
                                                                                                                             { return cutable_obj->qfn(p); },
                                                                                                                             0.0, std::array<DualContouring::Point3D, 2>{{{-1.1, -1.1, -1.1}, {1.1, 1.1, 1.1}}}, std::array<size_t, 3>{100, 100, 100}));
        ui->widget_3->set_obj(qm);
    */

    cutable_obj->set_prefered_settings();
    set_values_from_settings();
}

void main_window::slice_object()
{
    if (cutable_obj.get() == nullptr)
    {
        return;
    }

    slicer slicer1(cutable_obj);
    sliced_obj = std::make_shared<sliced_object>(slicer1.create_slices(settings::level_count, settings::inner_shell_count, settings::inner_shell_distance, [this](int v)
                                                                       { this->cb_slice_progressed(v); }));

    whole_obj = sliced_obj;

    show_support(ui->support_checkbox->isChecked());

    ui->verticalScrollBar->setMaximum(settings::level_count - 1);
    ui->verticalScrollBar->setValue(settings::level_count - 1);

    gcode_writer gw("output.gcode");
    gw.write_gcode(sliced_obj.get());
}

void main_window::generate_support()
{
    if (cutable_obj.get() == nullptr)
    {
        return;
    }
    if (sliced_obj.get() == nullptr)
    {
        return;
    }

    // support_obj = std::make_shared<support>(*(cutable_obj.get()));
    slicer slicer(cutable_obj);
    /*
    sliced_support = std::make_shared<sliced_object>(slicer.create_slices(
        settings::level_count, settings::inner_shell_count, settings::inner_shell_distance, [this](int v)
        { this->cb_slice_progressed(v); },
        true));
    */

    whole_obj = std::make_shared<sliced_object>(slicer.generate_support(*sliced_obj, settings::support_space_between, settings::level_count, settings::inner_shell_count, settings::inner_shell_distance, [this](int v)
                                                                        { this->cb_slice_progressed(v); }));

    std::shared_ptr<DualContouring::QuadMesh> qm = std::make_shared<DualContouring::QuadMesh>(DualContouring::isosurface([this](const DualContouring::Point3D &p)
                                                                                                                         { return cutable_obj->qfn(p); },
                                                                                                                         0.0, std::array<DualContouring::Point3D, 2>{{{-1.1, -1.1, -1.1}, {1.1, 1.1, 1.1}}}, std::array<size_t, 3>{100, 100, 100}));
    ui->widget_3->set_obj(qm);

    show_support(ui->support_checkbox->isChecked());
}

void main_window::show_support(int should)
{
    if (should)
    {
        printable = whole_obj;
    }
    else
    {
        printable = sliced_obj;
    }

    update_sliced_views();
}

void main_window::set_level_count(int n)
{
    settings::level_count = n;
}

void main_window::set_surface_type(int i)
{
    settings::s_type = i;
}

void main_window::set_inner_shell_count(int n)
{
    settings::inner_shell_count = n;
}

void main_window::set_inner_shell_distance(double v)
{
    settings::inner_shell_distance = v;
}

void main_window::set_infill_number_rot(int n)
{
    settings::infill_number_rot = n;
}

void main_window::cb_slice_progressed(int val)
{
    (main_window::slice_bar)->setValue(val);
}

void main_window::set_infill_space_between(double val)
{
    settings::infill_space_between = val;
}
