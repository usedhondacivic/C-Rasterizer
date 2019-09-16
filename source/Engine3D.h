#pragma once

class vec3d
{
public:
    union{
        struct{float x, y, z; };
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

    vec3d& operator+(const vec3d& rhs){
        vec3d r;
        r.x = this->x + rhs.x;
        r.y = this->y + rhs.y;
        r.z = this->z + rhs.z;
        return r;
    }

    vec3d& operator-(const vec3d& rhs){
        vec3d r;
        r.x = this->x - rhs.x;
        r.y = this->y - rhs.y;
        r.z = this->z - rhs.z;
        return r;
    }

    vec3d& operator*(const vec3d& rhs){
        vec3d r;
        r.x = this->x * rhs.x;
        r.y = this->y * rhs.y;
        r.z = this->z * rhs.z;
        return r;
    }

    vec3d& operator/(const vec3d& rhs){
        vec3d r;
        r.x = this->x / rhs.x;
        r.y = this->y / rhs.y;
        r.z = this->z / rhs.z;
        return r;
    }
};

struct triangle
{
    vec3d points[3] = {{}};

    vec3d color;
};

bool setup();

void update();