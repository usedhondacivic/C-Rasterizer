#pragma once

class vec3d
{
public:
    union{
        struct{float x, y, z; };
        struct{float pitch, yaw, roll; };
        float n[3] = {0, 0, 0};
    }

    vec3d(){
        x
    }
};

struct triangle
{
    vec3d points[3] = {0};

    vec3d color;
};

bool setup();

void update();