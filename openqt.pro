
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui opengl openglwidgets


CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
app/gui/main.cpp
app/gui/mainwindow.cpp
app/gui/myglwidget.cpp
app/slicer/slicer_module.cpp
app/slicer/generators/inner_shell_generator.cpp
app/slicer/generators/outer_shell_generator.cpp
app/slicer/generators/gentools/interval.cpp
app/slicer/generators/gentools/polylines.cpp
app/slicer/generators/gentools/section_organiser.cpp
app/slicer/global/section.cpp
app/slicer/object/RBF/FunctionCreator.cpp
app/slicer/object/RBF/NormalCreator.cpp
app/slicer/generators/infill_generator.cpp
app/slicer/generators/gentools/plane.cpp
app/slicer/printable_object.cpp


HEADERS += \
    mainwindow.h \
    myglwidget.h
    app/gui/myglwidget.h

FORMS += \
    app/gui/dialog.ui \
    app/gui/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
