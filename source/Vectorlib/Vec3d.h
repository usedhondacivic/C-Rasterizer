#pragma once

#include "Vectorlib/Matrix4x4.h"

class vec3d
{
public:
    union{
        struct{float x, y, z, w; };
        struct{float pitch, yaw, roll; };
        float n[3] = {0, 0, 0};
    };

    vec3d();

    vec3d(float a, float b, float c);

    float length() const;

    vec3d normal();

    float dot(const vec3d& b);
    
    float angleBetween(const vec3d& b);

    float prod(const vec3d& b);

    vec3d cross(const vec3d& b);

    vec3d& operator+=(const vec3d& rhs);

    vec3d& operator-=(const vec3d& rhs);

    vec3d& operator*=(const vec3d& rhs);

    vec3d& operator/=(const vec3d& rhs);

    vec3d& operator*=(const float rhs);

    vec3d& operator/=(const float rhs);

    vec3d operator+(const vec3d& rhs);

    vec3d operator-(const vec3d& rhs);

    vec3d operator*(const vec3d& rhs);

    vec3d operator/(const vec3d& rhs);

    vec3d operator*(const float rhs);

    vec3d operator/(const float rhs);

    vec3d operator*(const matrix4x4 rhs);
};