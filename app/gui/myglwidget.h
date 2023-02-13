#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "printable_object.h"

#include <iostream>

class poly_2D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject vao;
    std::vector<QOpenGLBuffer> vbos;
    QOpenGLShaderProgram *sp;

    sliced_obj *obj = nullptr;

    unsigned int printable_level = 0;

public:
    poly_2D_widget(QWidget *parent);

    void set_obj(sliced_obj *);

    ~poly_2D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

public slots:

    void slot_layer_changed(int l);
};
