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
}

void main_window::update_sliced_views()
{
    ui->widget->update();
    ui->widget_2->update();
}

// sliceolas nelkuli nezegetonel lesz ertelme
void main_window::load_object()
{

    sphere tsp;
    std::shared_ptr<DualContouring::QuadMesh> qm = std::make_shared<DualContouring::QuadMesh>(DualContouring::isosurface([&tsp](const DualContouring::Point3D &p)
                                                                                                                         { return tsp.qfn(p); },
                                                                                                                         0.0, std::array<DualContouring::Point3D, 2>{-1.1, 1.1}, std::array<size_t, 3>{100, 100, 100}));

    qm->writeOBJ("sppp.obj");

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

    support_obj = std::make_shared<support>(*(cutable_obj.get()));
    slicer slicer(support_obj);
    sliced_support = std::make_shared<sliced_object>(slicer.create_slices(settings::level_count, settings::inner_shell_count, settings::inner_shell_distance, [this](int v)
                                                                          { this->cb_slice_progressed(v); }));

    whole_obj = std::make_shared<sliced_object>(*sliced_obj, *sliced_support);

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
