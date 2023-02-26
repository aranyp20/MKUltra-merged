#include "poly_3D_widget.h"

poly_3D_widget::poly_3D_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

poly_3D_widget::~poly_3D_widget()
{
}

void poly_3D_widget::initializeGL()
{
    sp = new QOpenGLShaderProgram();
    sp->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                "#version 450\n"

                                "uniform mat4  V, P;\n"

                                "in vec3 position;\n"
                                "in vec3 color;\n"
                                "out vec4 fragColor;\n"

                                "vec4 pos = vec4(position,1);\n"

                                "void main(){\n"
                                "fragColor = vec4(color,1.0);\n"
                                "gl_Position = pos * V * P;\n"
                                "}");
    sp->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                "#version 450\n"
                                "in vec4 fragColor;\n"
                                "out vec4 finalColor;\n"
                                "void main(){\n"
                                "finalColor = fragColor;\n"
                                "}");
    sp->link();

    QMatrix4x4 q_v;
    QMatrix4x4 q_p;
    mat4 m_v = cam.V();
    mat4 m_p = cam.P();

    for (int i = 0; i < 4; i++)
    {
        q_v.setRow(i, QVector4D(m_v[i][0], m_v[i][1], m_v[i][2], m_v[i][3]));
        q_p.setRow(i, QVector4D(m_p[i][0], m_p[i][1], m_p[i][2], m_p[i][3]));
    }

    sp->bind();
    sp->setUniformValue("V", q_v);
    sp->setUniformValue("P", q_p);
    sp->release();

    glLineWidth(1);

    vao.create();
    vao.bind();
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    vao.release();
    vbo.release();

    glEnable(GL_DEPTH_TEST);
}

void poly_3D_widget::resizeGL(int w, int h)
{
}

void poly_3D_widget::paintGL()
{

    if (obj == nullptr)
        return;

    sp->bind();
    vao.bind();
    vbo.bind();

    const void *printable_data = colorize_level().data();

    vbo.allocate(printable_data, sizeof(float) * obj->filled_data_together.size() * 2);

    sp->enableAttributeArray("position");
    sp->enableAttributeArray("color");

    sp->setAttributeBuffer(0, GL_FLOAT, offsetof(qgl_vertex, position), 3, sizeof(qgl_vertex));
    sp->setAttributeBuffer(1, GL_FLOAT, offsetof(qgl_vertex, color), 3, sizeof(qgl_vertex));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINES, 0, obj->filled_data_together.size() * 2 / 3);
}

void poly_3D_widget::set_obj(sliced_object *_obj)
{
    // TODO: delete previus vaos, vbos
    obj = _obj;

    update();
}

void poly_3D_widget::slot_layer_changed(int l)
{
    if (obj == nullptr)
        return;

    printable_level = obj->slice_count - l - 1;
    update();
}

std::vector<qgl_vertex> poly_3D_widget::colorize_level() const
{
    std::vector<qgl_vertex> result;

    std::array<float, 3> color;
    for (unsigned int i = 0; i < obj->slice_count; i++)
    {
        i == printable_level ? color = {0.0f, 0.0f, 1.0f} : color = {0.0f, 1.0f, 0.0f};

        for (int j = 0; j < obj->filled_data[i].size(); j += 3)
        {
            result.push_back({QVector3D(obj->filled_data[i][j], obj->filled_data[i][j + 1], obj->filled_data[i][j + 2]), QVector3D(color[0], color[1], color[2])});
        }
    }

    return result;
}
