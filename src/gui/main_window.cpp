#include <QSpinBox>
#include "main_window.h"
#include "ui_main_window.h"
#include "poly_2D_widget.h"
#include "poly_3D_widget.h"
#include "slicer_module.h"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
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

    ui->surface_selector_box->addItem("Chmutov", QVariant(surface_type::CHMUTOV));
    ui->surface_selector_box->addItem("Gyroid", QVariant(surface_type::GYROID));
    QObject::connect(ui->surface_selector_box, qOverload<int>(&QComboBox::currentIndexChanged), this, &main_window::set_surface_type);

    set_values_from_settings();

    this->setStyleSheet("QMainWindow { background-color: grey; }");
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

// sliceolas nelkuli nezegetonel lesz ertelme
void main_window::load_object()
{
    if (cutable_obj != nullptr)
    {
        delete cutable_obj;
        cutable_obj = nullptr;
    }

    switch (settings::s_type)
    {
    case surface_type::CHMUTOV:
        cutable_obj = new chmutov();
        break;

    case surface_type::GYROID:
        cutable_obj = new gyroid();
        break;
    default:
        break;
    }

    cutable_obj->set_prefered_settings();
    set_values_from_settings();
}

void main_window::slice_object()
{
    if (cutable_obj == nullptr)
        return;

    if (sliced_obj != nullptr)
    {
        delete sliced_obj;
        sliced_obj = nullptr;
    }

    slicer slicer(cutable_obj);
    sliced_obj = new sliced_object(slicer.create_slices(settings::level_count, settings::inner_shell_count, settings::inner_shell_distance));

    ui->widget->set_obj(sliced_obj);

    ui->widget_2->set_obj(sliced_obj);

    ui->verticalScrollBar->setMaximum(settings::level_count - 1);
    ui->verticalScrollBar->setValue(settings::level_count - 1);
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

void main_window::set_infill_space_between(double val)
{
    settings::infill_space_between = val;
}
