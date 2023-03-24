

#include <QApplication>

#include "poly_2D_widget.h"
#include "main_window.h"
#include "ui_main_window.h"

#include "slicer_module.h"
#include "dual_num.h"

#include "cylinder.hpp"
#include "sphere.hpp"

int main(int argc, char *argv[])
{

    sphere sp;
    std::cout << sp.fn(interval(1, 2), interval(3, 4), 1) << std::endl;

    QApplication a(argc, argv);

    main_window w;

    w.show();

    return a.exec();
}
