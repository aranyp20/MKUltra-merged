

#include <QApplication>

#include "quadmesh.hh"

#include "poly_2D_widget.h"
#include "main_window.h"
#include "ui_main_window.h"

#include "slicer_module.h"
#include "dual_num.h"

#include "cylinder.hpp"
#include "sphere.hpp"

#include "iostream"

int main(int argc, char *argv[])
{
    DualContouring::QuadMesh tst;

    QApplication a(argc, argv);

    main_window w;

    w.show();

    return a.exec();
}
