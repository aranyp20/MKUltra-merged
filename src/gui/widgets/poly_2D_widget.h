#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <iostream>

#include "printable_object.h"

class poly_2D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram *sp;

    sliced_object *obj = nullptr;

    unsigned int printable_level = 0;

public:
    poly_2D_widget(QWidget *parent);

    void set_obj(sliced_object *);

    ~poly_2D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

public slots:

    void slot_layer_changed(int l);
};
