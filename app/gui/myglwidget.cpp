#include "myglwidget.h"

MyGLDrawer::MyGLDrawer(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

MyGLDrawer::~MyGLDrawer()
{
}

void MyGLDrawer::initializeGL()
{
    sp = new QOpenGLShaderProgram();
    sp->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                "#version 450\n"
                                "in vec3 position;\n"
                                "out vec4 fragColor;\n"
                                "void main(){\n"
                                "fragColor = vec4(0.0,1.0,0.0,1.0);\n"
                                "gl_Position = vec4(position,1.0);\n"
                                "}");
    sp->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                "#version 450\n"
                                "in vec4 fragColor;\n"
                                "out vec4 finalColor;\n"
                                "void main(){\n"
                                "finalColor = fragColor;\n"
                                "}");
    sp->link();

    float vertices[] = {
        0.0f, 0.8f, 0.0f,
        0.8f, -0.7f, 0.0f,
        -0.8f, -0.4f, 0.0f};
    vao.create();
    vao.bind();
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    vao.release();
    vbo.release();
}

void MyGLDrawer::resizeGL(int w, int h)
{
}

void MyGLDrawer::paintGL()
{

    sp->bind();
    vao.bind();
    vbo.bind();

    sp->enableAttributeArray("position");
    sp->setAttributeArray("position", GL_FLOAT, 0, 3);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}