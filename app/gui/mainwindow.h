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

    void load_object();
    void slice_object();

private:
    Ui::MainWindow *ui;

    frep_object *cutable_obj = nullptr;
};
#endif // MAINWINDOW_H
