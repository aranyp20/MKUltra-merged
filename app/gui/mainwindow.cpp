#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include "slicer_module.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frep_object *cutable_obj = new chmutov();
    slicer::bounding_box bb1(vec3(-1.10, -1.10, -1.10), 2.20, 2.20);
    slicer::bounding_box bb2(vec3(-110, -110, -110), 220, 220);
    slicer::bounding_box bb3(vec3(-11, -11, -11), 22, 22);

    slicer slicer(cutable_obj, bb1);

    int l = 20;

    ui->widget->set_obj(new sliced_obj(slicer.create_slices(l, 2, 0.08)));

    ui->verticalScrollBar->setMaximum(l - 1);
    ui->verticalScrollBar->setValue(l - 1);

    QObject::connect(ui->verticalScrollBar, &QScrollBar::valueChanged, ui->widget, &poly_2D_widget::slot_layer_changed);
}

MainWindow::~MainWindow()
{
    delete ui;
}
