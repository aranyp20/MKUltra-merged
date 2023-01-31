#pragma once

#include "NormalCreator.h"
#include <interval.h>


class FunctionCreator{

    static FunctionCreator* activeEntity;

    struct Node{
        vec3 pos;
        vec3 norm;

        double coefficient;
        double value;

        Node(const vec3&, double _value);
    };

    std::vector<Node> nodes;

    double preferableConstant = 50.0f;


    void FillNodes(const NormalCreator& nc);
    void AddNodeHelper(const vec3& _pos,const vec3& _normal);

    double KernelFunction(const vec3&, const vec3&) const;
    vec3 KernelFunction_grad(const vec3&, const vec3&) const;

    void CalculateCoefficients();


public:
    void Activate();

    FunctionCreator();


    static double Create(const vec3&);


    interval KernelFunction_interval(const interval& xi, const interval& yi, const vec3& p, double h) const;
    static interval Create_interval(const interval& i1, const interval& i2, double h);

    static vec3 Create_grad(const vec3& p);


};

