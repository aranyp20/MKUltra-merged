#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>

#include "printable_object.h"
#include "frep.hpp"
#include "gcode_writer.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

    static QProgressBar *slice_bar;
    static void cb_slice_progressed(int val);

private:
    Ui::MainWindow *ui;

    frep_object *cutable_obj = nullptr;
    sliced_object *sliced_obj = nullptr;

    gcode_writer g_writer;
public slots:
    void slice_object();
    void load_object();

    void set_values_from_settings();

    void set_level_count(int n);
    void set_surface_type(int i);
    void set_inner_shell_count(int n);
    void set_inner_shell_distance(double v);
    void set_infill_space_between(double val);
    void set_infill_number_rot(int n);
};
#endif // MAINWINDOW_H
