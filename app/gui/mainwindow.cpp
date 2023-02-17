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
