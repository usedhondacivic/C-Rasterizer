#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"

struct vec3d
{
    float x, y, z;
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
    float m[4][4] = {0};
};

mesh cubeMesh;

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

    float fNear = 0.1f; 
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)constants::SCREEN_HEIGHT / (float)constants::SCREEN_WIDTH;
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