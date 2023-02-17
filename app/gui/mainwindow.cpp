#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include "slicer_module.h"

#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// sliceolas nelkuli nezegetonel lesz ertelme
void MainWindow::load_object()
{
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

    slicer::bounding_box bb1(vec3(-1.10, -1.10, -1.10), 2.20, 2.20);
    slicer::bounding_box bb2(vec3(-110, -110, -110), 220, 220);
    slicer::bounding_box bb3(vec3(-11, -11, -11), 22, 22);

    slicer slicer(cutable_obj, bb1);

    ui->widget->set_obj(new sliced_obj(slicer.create_slices(settings::level_count, settings::inner_shell_count, settings::inner_shell_distance)));

    ui->verticalScrollBar->setMaximum(settings::level_count - 1);
    ui->verticalScrollBar->setValue(settings::level_count - 1);

    QObject::connect(ui->verticalScrollBar, &QScrollBar::valueChanged, ui->widget, &poly_2D_widget::slot_layer_changed);
}
