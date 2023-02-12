#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <iostream>

class poly_2D_widget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLShaderProgram *sp;

public:
    poly_2D_widget(QWidget *parent);

    ~poly_2D_widget();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;
};
