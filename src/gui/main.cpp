

#include <QApplication>

#include "poly_2D_widget.h"
#include "main_window.h"
#include "ui_main_window.h"

#include "slicer_module.h"
#include "dual_num.h"

#include "cylinder.hpp"

int main(int argc, char *argv[])
{

    cylinder cyl;
    std::cout << cyl.fn(interval(1), interval(2), 3) << std::endl;

    QApplication a(argc, argv);

    main_window w;

    w.show();

    return a.exec();
}
