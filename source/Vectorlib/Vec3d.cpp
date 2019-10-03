#include "Vectorlib/Matrix4x4.h"
#include "Vectorlib/Vec3d.h"

vec3d::vec3d(){
    x = y = z = 0;
    w = 1;
}

vec3d::vec3d(float a, float b, float c){
    x = a; y = b; z = c;
    w = 1;
}

float vec3d::length() const{
    return sqrtf(x * x + y * y + z * z);
}

vec3d vec3d::normal(){
    return *this / length();
}

float vec3d::dot(const vec3d& b){
    return x * b.x + y * b.y + z * b.z;
}

float vec3d::angleBetween(const vec3d& b){
    return acosf((x * b.x + y * b.y + z * b.z) / (length() * b.length()));
}

float vec3d::prod(const vec3d& b){
    return (x * b.x + y * b.y + z * b.z);
}

vec3d vec3d::cross(const vec3d& b){
    return {
        n[1] * b.n[2] - n[2] * b.n[1],
        n[2] * b.n[0] - n[0] * b.n[2],
        n[0] * b.n[1] - n[1] * b.n[0]
    };
}

vec3d& vec3d::operator+=(const vec3d& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

vec3d& vec3d::operator-=(const vec3d& rhs){
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

vec3d& vec3d::operator*=(const vec3d& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

vec3d& vec3d::operator/=(const vec3d& rhs){
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

vec3d& vec3d::operator*=(const float rhs){
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

vec3d& vec3d::operator/=(const float rhs){
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

vec3d vec3d::operator+(const vec3d& rhs){
    vec3d r;
    r.x = x + rhs.x;
    r.y = y + rhs.y;
    r.z = z + rhs.z;
    return r;
}

vec3d vec3d::operator-(const vec3d& rhs){
    vec3d r;
    r.x = x - rhs.x;
    r.y = y - rhs.y;
    r.z = z - rhs.z;
    return r;
}

vec3d vec3d::operator*(const vec3d& rhs){
    vec3d r;
    r.x = x * rhs.x;
    r.y = y * rhs.y;
    r.z = z * rhs.z;
    return r;
}

vec3d vec3d::operator/(const vec3d& rhs){
    vec3d r;
    r.x = x / rhs.x;
    r.y = y / rhs.y;
    r.z = z / rhs.z;
    return r;
}

vec3d vec3d::operator*(const float rhs){
    vec3d r;
    r.x = x * rhs;
    r.y = y * rhs;
    r.z = z * rhs;
    return r;
}

vec3d vec3d::operator/(const float rhs){
    vec3d r;
    r.x = x / rhs;
    r.y = y / rhs;
    r.z = z / rhs;
    return r;
}

vec3d vec3d::operator*(const matrix4x4 rhs){
    vec3d r;
    r.x = x * rhs.m[0][0] + y * rhs.m[1][0] + z * rhs.m[2][0] + w * rhs.m[3][0];
    r.y = x * rhs.m[0][1] + y * rhs.m[1][1] + z * rhs.m[2][1] + w * rhs.m[3][1];
    r.z = x * rhs.m[0][2] + y * rhs.m[1][2] + z * rhs.m[2][2] + w * rhs.m[3][2];
    r.w = x * rhs.m[0][3] + y * rhs.m[1][3] + z * rhs.m[2][3] + w * rhs.m[3][3];
    return r;
}