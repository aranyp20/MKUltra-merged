#pragma once
#include <vector>
#include "mymath.h"

class camera
{
    vec3 wEye, wUp, wLookat;
    float fov, aspect, fp, bp;

    float originalDistance;
    vec3 lastLookDir;

    void RecalculateLastLookDir();

public:
    camera();
    void replace(vec3 _wEye, vec3 _wLookat);
    void replace_eye(vec3 _wEye);
    void replace_lookat(vec3 _wLookat);

    std::vector<vec3> get_wuv() const;

    inline vec3 get_eye() const { return wEye; }
    inline vec3 get_lookat() const { return wLookat; }

    mat4 V() const;
    mat4 P() const;
};
