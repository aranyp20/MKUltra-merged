#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <iostream>

#include "printable_object.h"
#include "camera.h"

struct qgl_vertex
{
    QVector3D position;
    QVector3D color;
};

class poly_3D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram *sp;

    sliced_object *obj = nullptr;

    unsigned int printable_level = 0;

    camera cam;

    std::vector<qgl_vertex> colorize_level() const;

public:
    poly_3D_widget(QWidget *parent);

    void set_obj(sliced_object *);

    ~poly_3D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

public slots:

    void slot_layer_changed(int l);
};
