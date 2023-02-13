

#include <QApplication>

#include "myglwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slicer_module.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
