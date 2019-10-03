#include "Vectorlib/Matrix4x4.h"
#include "Vectorlib/Vec3d.h"

void matrix4x4::makeIdentity(){
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

void matrix4x4::makeRotationX(float fAngleRad){
    m[0][0] = 1;
    m[1][1] = cosf(fAngleRad * 0.5f);
    m[1][2] = sinf(fAngleRad * 0.5f);
    m[2][1] = -sinf(fAngleRad * 0.5f);
    m[2][2] = cosf(fAngleRad * 0.5f);
    m[3][3] = 1;
}

void matrix4x4::makeRotationZ(float fAngleRad){
    m[0][0] = cosf(fAngleRad);
    m[0][1] = sinf(fAngleRad);
    m[1][0] = -sinf(fAngleRad);
    m[1][1] = cosf(fAngleRad);
    m[2][2] = 1;
    m[3][3] = 1;
}

void matrix4x4::makeTranslation(float x, float y, float z){
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
}

void matrix4x4::makeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar){
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    m[0][0] = fAspectRatio * fFovRad;
    m[1][1] = fFovRad;
    m[2][2] = fFar / (fFar - fNear);
    m[3][2] = (-fFar * fNear) / (fFar -fNear);
    m[2][3] = 1.0f;
    m[3][3] = 0.0f;
}

void matrix4x4::makePointAt(vec3d &pos, vec3d &target, vec3d &up){
    
};

matrix4x4 matrix4x4::operator*(const matrix4x4 rhs){
    matrix4x4 matrix;
    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            matrix.m[r][c] = m[r][0] * rhs.m[0][c] + m[r][1] * rhs.m[1][c] + m[r][2] * rhs.m[2][c] + m[r][3] * rhs.m[3][c];
        }
    }
    return matrix;
}