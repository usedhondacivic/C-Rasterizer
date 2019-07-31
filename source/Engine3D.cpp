#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Engine3D.h"
#include "Main.h"

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
}

void update(){
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    for(auto tri : cubeMesh.triangles){
        
    }

    SDL_RenderPresent(gRenderer);
}