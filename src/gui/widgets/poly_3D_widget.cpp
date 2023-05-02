#include "poly_3D_widget.h"

poly_3D_widget::poly_3D_widget(QWidget *parent)
    : QOpenGLWidget(parent), mp_handler(this)
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

    if ((*obj).get() == nullptr)
        return;

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

    vao.bind();
    vbo.bind();

    std::vector<qgl_vertex> pp = colorize_level();
    const void *printable_data = pp.data();

    vbo.allocate(printable_data, sizeof(qgl_vertex) * pp.size());

    sp->enableAttributeArray("position");
    sp->enableAttributeArray("color");

    sp->setAttributeBuffer(0, GL_FLOAT, offsetof(qgl_vertex, position), 3, sizeof(qgl_vertex));
    sp->setAttributeBuffer(1, GL_FLOAT, offsetof(qgl_vertex, color), 3, sizeof(qgl_vertex));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINES, 0, pp.size());
}

void poly_3D_widget::set_obj(std::shared_ptr<sliced_object> *_obj)
{
    // TODO: delete previus vaos, vbos
    obj = _obj;

    update();
}

void poly_3D_widget::slot_layer_changed(int l)
{
    if ((*obj).get() == nullptr)
        return;

    printable_level = (*obj)->get_slice_count() - l - 1;
    update();
}

std::vector<qgl_vertex> poly_3D_widget::colorize_level() const
{
    std::vector<qgl_vertex> result;

    std::array<float, 3> color;
    for (unsigned int i = 0; i < (*obj)->get_slice_count(); i++)
    {
        std::vector<qgl_vertex> level;

        if (i == printable_level)
        {
            level = (*obj)->get_custom_colored_level(i, vec3(1, 1, 1));
        }
        else
        {
            level = (*obj)->get_colored_level(i);
        }

        result.insert(result.end(), level.begin(), level.end());
    }

    return result;
}

void poly_3D_widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mp_handler.register_pos();
    }
}

void poly_3D_widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        rotate_camera();
        mp_handler.register_pos();
        update();
    }
}

void poly_3D_widget::zoom_camera(int n)
{
    vec3 move_axis = normalize(cam.get_eye() - cam.get_lookat());
    cam.replace_eye(cam.get_lookat() + move_axis * (4.0f - n / 40.0f));
    update();
}

void poly_3D_widget::rotate_camera()
{
    vec2 last_pos(mp_handler.get_last_pos());
    vec2 curr_pos(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());

    if (last_pos.x == curr_pos.x && last_pos.y == curr_pos.y)
        return;

    vec2 tVec = curr_pos - last_pos;

    std::vector<vec3> wuv = cam.get_wuv();
    vec3 axis = wuv[2] * tVec.x + wuv[1] * tVec.y;
    vec3 newEye = cam.get_eye();
    newEye = newEye - cam.get_lookat();
    TransformPoint(newEye, RotationMatrix(tVec.length() / -200, axis));
    newEye = newEye + cam.get_lookat();
    cam.replace_eye(newEye);
}

mouse_pos_handler::mouse_pos_handler(QWidget *_parent) : parent(_parent)
{
}

void mouse_pos_handler::register_pos()
{
    last_pos = vec2(parent->mapFromGlobal(QCursor::pos()).x(), parent->mapFromGlobal(QCursor::pos()).y());
}

vec2 mouse_pos_handler::get_last_pos() const
{
    return last_pos;
}
