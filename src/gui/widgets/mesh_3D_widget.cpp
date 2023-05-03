#include "mesh_3D_widget.h"

std::vector<qgl_vertex> mesh_3D_widget::obj_to_printable() const
{
    std::vector<qgl_vertex> result;

    for (const auto &a : obj->quads)
    {
        vec3 normal = cross(vec3(obj->points[a[1]-1][0], obj->points[a[1]-1][1], obj->points[a[1]-1][2]) - vec3(obj->points[a[0]-1][0], obj->points[a[0]-1][1], obj->points[a[0]-1][2]), vec3(obj->points[a[2]-1][0], obj->points[a[2]-1][1], obj->points[a[2]-1][2]) - vec3(obj->points[a[0]-1][0], obj->points[a[0]-1][1], obj->points[a[0]-1][2]));
        result.push_back({QVector3D{obj->points[a[0]-1][0], obj->points[a[0]-1][1], obj->points[a[0]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
        result.push_back({QVector3D{obj->points[a[1]-1][0], obj->points[a[1]-1][1], obj->points[a[1]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
        result.push_back({QVector3D{obj->points[a[2]-1][0], obj->points[a[2]-1][1], obj->points[a[2]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
        result.push_back({QVector3D{obj->points[a[0]-1][0], obj->points[a[0]-1][1], obj->points[a[0]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
        result.push_back({QVector3D{obj->points[a[2]-1][0], obj->points[a[2]-1][1], obj->points[a[2]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
        result.push_back({QVector3D{obj->points[a[3]-1][0], obj->points[a[3]-1][1], obj->points[a[3]-1][2]}, QVector3D{normal.x, normal.y, normal.z}});
    }



    return result;
}

mesh_3D_widget::mesh_3D_widget(QWidget *parent)
    : QOpenGLWidget(parent), mp_handler(this)
{
}

void mesh_3D_widget::set_obj(std::shared_ptr<DualContouring::QuadMesh> _obj)
{
    obj = _obj;
    update();
}

mesh_3D_widget::~mesh_3D_widget()
{
}

void mesh_3D_widget::initializeGL()
{
    sp = new QOpenGLShaderProgram();
    sp->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                "#version 450\n"

                                "uniform mat4  V, P;\n"
                                "uniform vec3 eye;\n"

                                "in vec3 position;\n"
                                "in vec3 normal;\n"
                                "out vec3 fragColor;\n"

                                "void main(){\n"
                                "vec3 lightPos = vec3(-3.0,-1.0,-1.0);\n"

                                "vec4 pos = vec4(position,1);\n"
                                "vec4 norm = vec4(normal,0);\n"
                                "gl_Position = pos * V * P;\n"

                                "vec3 V = normalize(eye-position);\n"
                                "vec3 N = normalize(position);\n"
                                "vec3 L = normalize(lightPos-position);\n"

                                "fragColor = vec3(0.1,0.2,0.2) + vec3(0.5,0.5,0.5)*max(dot(N,L),0);\n"
                                "}");
    sp->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                "#version 450\n"
                                "in vec3 fragColor;\n"
                                "out vec4 finalColor;\n"
                                "void main(){\n"
                                "finalColor = vec4(fragColor,1.0);\n"
                                "}");
    sp->link();

    vao.create();
    vao.bind();
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    vao.release();
    vbo.release();

    glEnable(GL_DEPTH_TEST);
}

void mesh_3D_widget::resizeGL(int w, int h)
{
}

void mesh_3D_widget::paintGL()
{

    if (obj.get() == nullptr)
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
    sp->setUniformValue("eye", QVector3D(cam.get_eye().x, cam.get_eye().y, cam.get_eye().z));

    vao.bind();
    vbo.bind();

    std::vector<qgl_vertex> pp = obj_to_printable();
    const void *printable_data = pp.data();

    vbo.allocate(printable_data, sizeof(qgl_vertex) * pp.size());

    sp->enableAttributeArray("position");
    sp->enableAttributeArray("normal");

    sp->setAttributeBuffer(0, GL_FLOAT, offsetof(qgl_vertex, position), 3, sizeof(qgl_vertex));
    sp->setAttributeBuffer(1, GL_FLOAT, offsetof(qgl_vertex, color), 3, sizeof(qgl_vertex));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, pp.size());
}

// common with poly_3D_widget
void mesh_3D_widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mp_handler.register_pos();
    }
}

void mesh_3D_widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        rotate_camera();
        mp_handler.register_pos();
        update();
    }
}

void mesh_3D_widget::rotate_camera()
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

// ///////////////