#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"
#include "Graphics.h"

struct vec3d
{
    float x, y, z;
};

struct triangle
{
    vec3d points[3];
};

struct mesh
{
    std::vector<triangle> triangles;
};

struct matrix4x4
{
    float m[4][4] = {0};
};

mesh cubeMesh;
matrix4x4 projectionMatrix;

void MultiplyMatrixVector(vec3d &input, vec3d &output, matrix4x4 &matrix){
    output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
    output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
    output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
    float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];
    if(w != 0.0f){
        output.x /= w;
        output.y /= w;
        output.z /= w;
    }
}

void setup(){
    cubeMesh.triangles = {
        //South
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},

        //East
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},

        //North
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},

        //West
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        
        //Top
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        
        //Bottom
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f},
    };

    //Projection Matrix
    float fNear = 0.1f; 
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)constants::SCREEN_HEIGHT / (float)constants::SCREEN_WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * M_PI);

    projectionMatrix.m[0][0] = fAspectRatio * fFovRad;
    projectionMatrix.m[1][1] = fFovRad;
    projectionMatrix.m[2][2] = fFar / (fFar - fNear);
    projectionMatrix.m[3][2] = (-fFar * fNear) / (fFar -fNear);
    projectionMatrix.m[2][3] = 1.0f;
    projectionMatrix.m[3][3] = 0.0f;
}

void update(){
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );        
    SDL_RenderDrawLine( gRenderer, 0, constants::SCREEN_HEIGHT / 2, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT / 2 );

    for(auto tri : cubeMesh.triangles){
        triangle triProjected;
        MultiplyMatrixVector(tri.points[0], triProjected.points[0], projectionMatrix);
        MultiplyMatrixVector(tri.points[1], triProjected.points[1], projectionMatrix);
        MultiplyMatrixVector(tri.points[2], triProjected.points[2], projectionMatrix);

    }

    SDL_RenderPresent(gRenderer);
}