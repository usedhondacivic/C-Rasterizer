#pragma once

struct vec3d
{
    float x, y, z;
};

struct triangle
{
    vec3d points[3];

    vec3d color;
};

bool setup();

void update();