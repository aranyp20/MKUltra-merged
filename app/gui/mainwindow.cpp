#include <QSpinBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include "slicer_module.h"

#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->verticalScrollBar, &QScrollBar::valueChanged, ui->widget, &poly_2D_widget::slot_layer_changed);
    QObject::connect(ui->surface_loader_button, &QPushButton::pressed, this, &MainWindow::load_object);
    QObject::connect(ui->slice_button, &QPushButton::pressed, this, &MainWindow::slice_object);

    ui->layer_count_box->setValue(settings::level_count);
    QObject::connect(ui->layer_count_box, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::set_level_count);

    ui->surface_selector_box->addItem("Chmutov", QVariant(surface_type::CHMUTOV));
    ui->surface_selector_box->addItem("Gyroid", QVariant(surface_type::GYROID));
    QObject::connect(ui->surface_selector_box, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::set_surface_type);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// sliceolas nelkuli nezegetonel lesz ertelme
void MainWindow::load_object()
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
    default:
        break;
    }
}

void MainWindow::slice_object()
{
    if (cutable_obj == nullptr)
        return;

    if (sliced_obj != nullptr)
    {
        delete sliced_obj;
        sliced_obj = nullptr;
    }

    slicer::bounding_box bb1(vec3(-1.10, -1.10, -1.10), 2.20, 2.20);
    slicer::bounding_box bb2(vec3(-110, -110, -110), 220, 220);
    slicer::bounding_box bb3(vec3(-11, -11, -11), 22, 22);

    slicer slicer(cutable_obj, bb1);
    sliced_obj = new sliced_object(slicer.create_slices(settings::level_count, settings::inner_shell_count, settings::inner_shell_distance));

    ui->widget->set_obj(sliced_obj);

    ui->verticalScrollBar->setMaximum(settings::level_count - 1);
    ui->verticalScrollBar->setValue(settings::level_count - 1);
}

void MainWindow::set_level_count(int n)
{
    settings::level_count = n;
}

void MainWindow::set_surface_type(int i)
{
    settings::s_type = i;
}
