

#include <QApplication>

#include "myglwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slicer_module.h"

int main(int argc, char *argv[])
{
    frep_object *cutable_obj = new chmutov();
    slicer::bounding_box bb1(vec3(-1.10, -1.10, -1.10), 2.20, 2.20);
    slicer::bounding_box bb2(vec3(-110, -110, -110), 220, 220);
    slicer::bounding_box bb3(vec3(-11, -11, -11), 22, 22);

    slicer slicer(cutable_obj, bb1);
    slicer.create_slices(10, 2, 1);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
