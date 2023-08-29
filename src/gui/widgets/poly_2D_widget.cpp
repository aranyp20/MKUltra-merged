#include "poly_2D_widget.h"

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
                                "in vec3 color;\n"
                                "out vec4 fragColor;\n"
                                "void main(){\n"
                                "fragColor = vec4(color,1.0);\n"
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

    glLineWidth(1);

    vao.create();
    vao.bind();
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    vao.release();
    vbo.release();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void poly_2D_widget::resizeGL(int w, int h)
{
}

void poly_2D_widget::paintGL()
{

    if ((*obj) == nullptr)
        return;

    sp->bind();
    vao.bind();
    vbo.bind();

    std::vector<qgl_vertex> pp = (*obj)->get_colored_separated_level(printable_level);
    const void *printable_data = pp.data();

    vbo.allocate(printable_data, sizeof(qgl_vertex) * pp.size());

    sp->enableAttributeArray("position");
    sp->enableAttributeArray("color");

    sp->setAttributeBuffer(0, GL_FLOAT, offsetof(qgl_vertex, position), 3, sizeof(qgl_vertex));
    sp->setAttributeBuffer(1, GL_FLOAT, offsetof(qgl_vertex, color), 3, sizeof(qgl_vertex));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINES, 0, pp.size());
}

void poly_2D_widget::set_obj(std::shared_ptr<sliced_object> *_obj)
{
    // TODO: delete previus vaos, vbos
    obj = _obj;

    update();
}

void poly_2D_widget::slot_layer_changed(int l)
{
    if ((*obj).get() == nullptr)
        return;
    printable_level = (*obj)->get_slice_count() - l - 1;
    update();
}
