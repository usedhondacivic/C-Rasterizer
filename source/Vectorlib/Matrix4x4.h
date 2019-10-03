#pragma once

#include <math.h>
#include <Vectorlib/Vec3d.h>

class matrix4x4
{
public:
    float m[4][4] = {0};

    matrix4x4();

    void makeIdentity();

    void makeRotationX(float fAngleRad);

    void makeRotationZ(float fAngleRad);

    void makeTranslation(float x, float y, float z);

    void makeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

    void makePointAt(vec3d &pos, vec3d &target, vec3d &up);

    matrix4x4 operator*(const matrix4x4 rhs);
};