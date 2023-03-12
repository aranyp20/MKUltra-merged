

#include <QApplication>

#include "poly_2D_widget.h"
#include "main_window.h"
#include "ui_main_window.h"

#include "slicer_module.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    main_window w;
    w.show();

    return a.exec();
}
