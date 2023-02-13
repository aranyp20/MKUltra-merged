#include "myglwidget.h"

poly_2D_widget::poly_2D_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

poly_2D_widget::~poly_2D_widget()
{
}

void poly_2D_widget::initializeGL()
{
    sp = new QOpenGLShaderProgram();
    sp->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                "#version 450\n"
                                "in vec3 position;\n"
                                "out vec4 fragColor;\n"
                                "void main(){\n"
                                "fragColor = vec4(0.0,1.0,0.0,1.0);\n"
                                "gl_Position = vec4(position.x,position.y,0.0,1.0);\n"
                                "}");
    sp->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                "#version 450\n"
                                "in vec4 fragColor;\n"
                                "out vec4 finalColor;\n"
                                "void main(){\n"
                                "finalColor = fragColor;\n"
                                "}");
    sp->link();

    glLineWidth(2);

    vao.create();
    vao.bind();
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    vao.release();
    vbo.release();
}

void poly_2D_widget::resizeGL(int w, int h)
{
}

void poly_2D_widget::paintGL()
{

    if (surface == nullptr)
        return;

    int seg = 8;
    int sel = 0;

    float vertices[surface->data[seg].data[sel].size() * 3];
    int tmp = 0;
    for (int i = 0; i < surface->data[seg].data[sel].size(); i++, tmp += 3)
    {
        vertices[tmp] = (float)(surface->data[seg].data[sel][i].x);
        vertices[tmp + 1] = (float)(surface->data[seg].data[sel][i].y);
        vertices[tmp + 2] = (float)(surface->data[seg].data[sel][i].z);
    }

    vao.bind();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    sp->bind();
    vao.bind();
    vbo.bind();

    sp->enableAttributeArray("position");
    sp->setAttributeArray("position", GL_FLOAT, 0, 3);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINE_STRIP, 0, surface->data[seg].data[sel].size());
}

void poly_2D_widget::set_surface(sliced_obj *_surface)
{
    surface = _surface;
}
