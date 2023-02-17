#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "printable_object.h"
#include "frep.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    frep_object *cutable_obj = nullptr;
    sliced_object *sliced_obj = nullptr;
public slots:
    void slice_object();
    void load_object();

    void set_level_count(int n);
    void set_surface_type(int i);
};
#endif // MAINWINDOW_H
