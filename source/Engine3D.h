#pragma once

struct vec3d
{
    float x, y, z;
};

struct triangle
{
    vec3d points[3] = {0};

    vec3d color;
};

bool setup();

void update();