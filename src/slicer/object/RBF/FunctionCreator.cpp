#include <limits>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <eigen3/Eigen/Dense>
#include "FunctionCreator.h"

// FunctionCreator *FunctionCreator::activeEntity = NULL;

FunctionCreator::FunctionCreator()
{

    NormalCreator n1("/home/peti/Downloads/sphere.obj");

    FillNodes(n1);

    CalculateCoefficients();

    std::cout << "Surface creation done." << std::endl;
}

void FunctionCreator::FillNodes(const NormalCreator &nc)
{

    std::vector<vec3> verts = nc.GetVertices();
    std::vector<vec3> norms = nc.GetNormals();

    for (int i = 0; i < verts.size(); i++)
    {
        AddNodeHelper(verts[i], norms[i]);
    }
}

void FunctionCreator::CalculateCoefficients()
{
    unsigned int sh = nodes.size();
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(sh, sh);
    for (int i = 0; i < sh; i++)
    {
        for (int j = 0; j < sh; j++)
        {
            A(i, j) = KernelFunction(nodes[i].pos, nodes[j].pos);
        }
    }
    Eigen::VectorXf b = Eigen::VectorXf::Random(sh);
    Eigen::VectorXf c = Eigen::VectorXf::Random(sh);
    for (int i = 0; i < sh; i++)
    {
        c(i) = nodes[i].value;
    }

    b = A.colPivHouseholderQr().solve(c);

    for (int i = 0; i < sh; i++)
    {
        nodes[i].coefficient = b[i];
    }
}

void FunctionCreator::AddNodeHelper(const vec3 &_pos, const vec3 &_normal)
{
    const double currentH = 0.2;
    nodes.push_back(Node(_pos, 0));
    nodes.push_back(Node(_pos + _normal * currentH, currentH));
    nodes.push_back(Node(_pos - _normal * currentH, -currentH));
}

FunctionCreator::Node::Node(const vec3 &p, double _value) : pos(p), value(_value)
{
}

////////////////////////////////sima///////////////////////////////////////////////////////////////////

double FunctionCreator::KernelFunction(const vec3 &p1, const vec3 &p2) const
{
    double influenceZone = 0.3;
    double normalizedDistance = (p1-p2).length() / influenceZone;
    if(normalizedDistance<0 || normalizedDistance>1) return 0;
    return pow(1-normalizedDistance,4) * (4 * normalizedDistance + 1);

////////
    double preferableConstant = 50.0f;
    double distance = (p1 - p2).length();

    return sqrt(distance * distance + preferableConstant * preferableConstant);
}

double FunctionCreator::Create(const vec3 &p) const
{

    double result = 0;
    for (int i = 0; i < nodes.size(); i++)
    {
        result += nodes[i].coefficient * KernelFunction(p, nodes[i].pos);
    }

    return result;
}
///////////////////////////////interval////////////////////////

interval FunctionCreator::KernelFunction_interval(const interval &xi, const interval &yi, const vec3 &p, double h) const
{
    interval distance(std::min({(vec3(xi.start, yi.start, h) - p).length(), (vec3(xi.end, yi.start, h) - p).length(), (vec3(xi.start, yi.end, h) - p).length(), (vec3(xi.end, yi.end, h) - p).length()}), std::max({(vec3(xi.start, yi.start, h) - p).length(), (vec3(xi.end, yi.start, h) - p).length(), (vec3(xi.start, yi.end, h) - p).length(), (vec3(xi.end, yi.end, h) - p).length()}));

    interval influenceZone(0.2);
    interval normalizedDistance = distance / influenceZone;
    

    if(interval(1)<normalizedDistance) return interval(0);
    

    return pow(interval(1)-normalizedDistance,4) * (interval(4) * normalizedDistance + interval(1));

///
    return sqrt(distance * distance + interval(preferableConstant) * interval(preferableConstant));
}

interval FunctionCreator::Create_interval(const interval &i1, const interval &i2, double h) const
{
    interval result(0);
    for (int i = 0; i < nodes.size(); i++)
    {
        result = result + interval(nodes[i].coefficient) * KernelFunction_interval(i1, i2, nodes[i].pos, h);
    }

    return result;
}
///////////////////////////////grad///////////////////////////

vec3 FunctionCreator::KernelFunction_grad(const vec3 &p1, const vec3 &p2) const
{
    double dist = distance(p1, p2);
    return vec3((p1.x - p2.x) / dist, (p1.y - p2.y) / dist, (p1.z - p2.z) / dist) * preferableConstant;
}

vec3 FunctionCreator::Create_grad(const vec3 &p) const
{
    vec3 result;
    for (auto &a : nodes)
    {
        result = result + KernelFunction_grad(p, a.pos) * a.coefficient;
    }
    return result;
}