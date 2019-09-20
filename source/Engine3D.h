#pragma once
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class vec3d
{
public:
    union{
        struct{float x, y, z, w; };
        struct{float pitch, yaw, roll; };
        float n[3] = {0, 0, 0};
    };

    vec3d(){
        x = y = z = 0;
    }

    vec3d(float a, float b, float c){
        x = a; y = b; z = c;
    }

    float length() const{
        return sqrtf(x * x + y * y + z * z);
    }

    vec3d normal(){
        return *this / length();
    }

    float dot(const vec3d& b){
        return acosf((x * b.x + y * b.y + z * b.z) / (length() * b.length()));
    }

    float prod(const vec3d& b){
        return (x * b.x + y * b.y + z * b.z);
    }

    vec3d cross(const vec3d& b){
        return {
            n[1] * b.n[2] - n[2] * b.n[1],
            n[2] * b.n[0] - n[0] * b.n[2],
            n[0] * b.n[1] - n[1] * b.n[0]
        };
    }

    vec3d& operator +=(const vec3d& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    vec3d& operator-=(const vec3d& rhs){
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    vec3d& operator*=(const float rhs){
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    vec3d& operator/=(const float rhs){
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        return *this;
    }

    vec3d operator+(const vec3d& rhs){
        vec3d r;
        r.x = this->x + rhs.x;
        r.y = this->y + rhs.y;
        r.z = this->z + rhs.z;
        return r;
    }

    vec3d operator-(const vec3d& rhs){
        vec3d r;
        r.x = this->x - rhs.x;
        r.y = this->y - rhs.y;
        r.z = this->z - rhs.z;
        return r;
    }

    vec3d operator*(const vec3d& rhs){
        vec3d r;
        r.x = this->x * rhs.x;
        r.y = this->y * rhs.y;
        r.z = this->z * rhs.z;
        return r;
    }

    vec3d operator/(const vec3d& rhs){
        vec3d r;
        r.x = this->x / rhs.x;
        r.y = this->y / rhs.y;
        r.z = this->z / rhs.z;
        return r;
    }

    vec3d operator*(const float rhs){
        vec3d r;
        r.x = this->x * rhs;
        r.y = this->y * rhs;
        r.z = this->z * rhs;
        return r;
    }

    vec3d operator/(const float rhs){
        vec3d r;
        r.x = this->x / rhs;
        r.y = this->y / rhs;
        r.z = this->z / rhs;
        return r;
    }
};

struct triangle
{
    vec3d points[3] = {{}};

    vec3d color;
};

struct mesh
{
    std::vector<triangle> triangles;

    bool LoadFromObjectFile(std::string sFilename){
        std::ifstream f(sFilename);
        if(!f.is_open()){
            return false;
        }

        std::vector<vec3d> verts;

        while(!f.eof()){
            char line[128];
            f.getline(line, 128);

            std::stringstream s;
            s << line;

            char junk;

            if(line[0] == 'v'){
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if(line[0] == 'f'){
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                triangles.push_back({ verts[f[0] - 1],  verts[f[1] - 1], verts[f[2] - 1]});
            }
        }

        return true;
    }
};

enum class matrixType
{
    IDENTITY,
    ROTATION_X,
    ROTATION_Z,
    TRANSLATION,
    PROJECTION
};

class matrix4x4
{
public:
    float m[4][4] = {0};

    matrix4x4(){
    }

    void makeIdentity(){
        this->m[0][0] = 1.0f;
        this->m[1][1] = 1.0f;
        this->m[2][2] = 1.0f;
        this->m[3][3] = 1.0f;
    }

    void makeRotationX(float fAngleRad){
        this->m[0][0] = 1;
        this->m[1][1] = cosf(fAngleRad * 0.5f);
        this->m[1][2] = sinf(fAngleRad * 0.5f);
        this->m[2][1] = -sinf(fAngleRad * 0.5f);
        this->m[2][2] = cosf(fAngleRad * 0.5f);
        this->m[3][3] = 1;
    }

    void makeRotationZ(float fAngleRad){
        this->m[0][0] = cosf(fAngleRad);
        this->m[0][1] = sinf(fAngleRad);
        this->m[1][0] = -sinf(fAngleRad);
        this->m[1][1] = cosf(fAngleRad);
        this->m[2][2] = 1;
        this->m[3][3] = 1;
    }

    void makeTranslation(float x, float y, float z){
        this->m[0][0] = 1.0f;
        this->m[1][1] = 1.0f;
        this->m[2][2] = 1.0f;
        this->m[3][3] = 1.0f;
        this->m[3][0] = x;
        this->m[3][1] = y;
        this->m[3][2] = z;
    }

    void makeProjection(float fNear, float fFar, float fFov, float fAspectRatio, float fFovRad){
        this->m[0][0] = fAspectRatio * fFovRad;
        this->m[1][1] = fFovRad;
        this->m[2][2] = fFar / (fFar - fNear);
        this->m[3][2] = (-fFar * fNear) / (fFar -fNear);
        this->m[2][3] = 1.0f;
        this->m[3][3] = 0.0f;
    }
};

bool setup();

void update();