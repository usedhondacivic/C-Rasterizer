#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"

struct vec3d
{
    double x, y, z;
};

struct triangle
{
    vec3d point[3];
};

struct mesh
{
    std::vector<triangle> triangles;
};

struct matrix4x4
{
    double m[4][4] = {0};
};

mesh cubeMesh;
matrix4x4 projectionMatrix;

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
}

void update(){
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );        
    SDL_RenderDrawLine( gRenderer, 0, constants::SCREEN_HEIGHT / 2, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT / 2 );

    for(auto tri : cubeMesh.triangles){
        
    }

    SDL_RenderPresent(gRenderer);
}