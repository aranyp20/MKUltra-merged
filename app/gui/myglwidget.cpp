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

    glLineWidth(1);

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

    if (obj == nullptr)
        return;

    sp->bind();
    vao.bind();
    vbo.bind();
    vbo.allocate(obj->filled_data[printable_level].data(), sizeof(float) * obj->filled_data[printable_level].size());

    sp->enableAttributeArray("position");
    sp->setAttributeArray("position", GL_FLOAT, 0, 3);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_LINES, 0, obj->filled_data[printable_level].size() / 3);
}

void poly_2D_widget::set_obj(sliced_obj *_obj)
{
    // TODO: delete previus vaos, vbos
    obj = _obj;

    /*
    for (int i = 5; i < 6; i++)
    {
        QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
        vbo.create();
        vbo.bind();
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        std::vector<float> xd;
        for (int j = 0; j < 12; j++)
        {
            std::cout << obj->filled_data[i][j] << std::endl;
            xd.push_back((float)obj->filled_data[i][j]);
        }
        vbo.allocate(xd.data(), xd.size() * sizeof(float));

        vbos.push_back(vbo);
    }
*/
}

void poly_2D_widget::slot_layer_changed(int l)
{
    printable_level = obj->slice_count - l - 1;
    update();
}
