#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMouseEvent>

#include <iostream>

#include "printable_object.h"
#include "camera.h"

class mouse_pos_handler
{
    QWidget *parent;
    vec2 last_pos;

public:
    mouse_pos_handler(QWidget *);

    void register_pos();

    vec2 get_last_pos() const;
};

class poly_3D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram *sp;

    sliced_object **obj = nullptr;

    unsigned int printable_level = 0;

    camera cam;

    std::vector<qgl_vertex> colorize_level() const;

    mouse_pos_handler mp_handler;

    void rotate_camera();

public:
    poly_3D_widget(QWidget *parent);

    void set_obj(sliced_object **);

    ~poly_3D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

public slots:

    void slot_layer_changed(int l);
};
