#pragma once

#include "quadmesh.hh"

#include "poly_3D_widget.h"

class mesh_3D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram *sp;

    camera cam;

    mouse_pos_handler mp_handler;

    std::shared_ptr<DualContouring::QuadMesh> obj;
    std::vector<qgl_vertex> obj_to_printable() const;

    void rotate_camera();

public:
    mesh_3D_widget(QWidget *parent);

    void set_obj(std::shared_ptr<DualContouring::QuadMesh>);

    ~mesh_3D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};