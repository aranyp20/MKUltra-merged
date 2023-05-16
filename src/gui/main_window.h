#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QProgressBar>

#include "printable_object.h"
#include "frep.hpp"
#include "gcode_writer.h"
#include "support.hpp"

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
    void cb_slice_progressed(int val);

private:
    Ui::MainWindow *ui;

    std::shared_ptr<frep_object> cutable_obj;
    // std::shared_ptr<support> support_obj;

    std::shared_ptr<sliced_object> sliced_obj;
    // std::shared_ptr<sliced_object> sliced_support;
    std::shared_ptr<sliced_object> whole_obj;
    std::shared_ptr<sliced_object> printable;

    gcode_writer g_writer;

    void update_sliced_views();
public slots:
    void slice_object();
    void load_object();
    void generate_support();
    void show_support(int should);

    void set_values_from_settings();

    void set_level_count(int n);
    void set_surface_type(int i);
    void set_inner_shell_count(int n);
    void set_inner_shell_distance(double v);
    void set_infill_space_between(double val);
    void set_infill_number_rot(int n);
};
#endif // MAINWINDOW_H
