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
                                "out vec4 fragColor;\n"

                                "vec4 pos = vec4(position,1);\n"

                                "void main(){\n"
                                "fragColor = vec4(0.0,1.0,0.0,1.0);\n"
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

    vbo.allocate(obj->filled_data_together.data(), sizeof(float) * obj->filled_data_together.size());

    sp->enableAttributeArray("position");
    sp->setAttributeArray("position", GL_FLOAT, 0, 3);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINES, 0, obj->filled_data_together.size() / 3);
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
