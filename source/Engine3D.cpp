#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"
#include "Graphics.h"

struct mesh
{
    std::vector<triangle> triangles;
};

struct matrix4x4
{
    float m[4][4] = {0};
};

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

mesh cubeMesh;
matrix4x4 projectionMatrix;
matrix4x4 rotationMatrixX, rotationMatrixZ;
float fTheta;

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

bool setup(){
    cubeMesh.triangles = {
        //South
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		//East                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		//North                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		//West                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		//Top                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		//Bottom                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    };

    //Projection Matrix
    float fNear = 0.1f; 
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)constants::SCREEN_HEIGHT / (float)constants::SCREEN_WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    projectionMatrix.m[0][0] = fAspectRatio * fFovRad;
    projectionMatrix.m[1][1] = fFovRad;
    projectionMatrix.m[2][2] = fFar / (fFar - fNear);
    projectionMatrix.m[3][2] = (-fFar * fNear) / (fFar -fNear);
    projectionMatrix.m[2][3] = 1.0f;
    projectionMatrix.m[3][3] = 0.0f;

    return true;
}

void update(){
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    float fElapsedTime = elapsedTime.count();

    fTheta += 1.0f *fElapsedTime;

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );

    //X rotation matrix
    rotationMatrixZ.m[0][0] = cosf(fTheta);
    rotationMatrixZ.m[0][1] = sinf(fTheta);
    rotationMatrixZ.m[1][0] = -sinf(fTheta);
    rotationMatrixZ.m[1][1] = cosf(fTheta);
    rotationMatrixZ.m[2][2] = 1;
    rotationMatrixZ.m[3][3] = 1;

    //X rotation matrix
    rotationMatrixX.m[0][0] = 1;
    rotationMatrixX.m[1][1] = cosf(fTheta * 0.5f);
    rotationMatrixX.m[1][2] = sinf(fTheta * 0.5f);
    rotationMatrixX.m[2][1] = -sinf(fTheta * 0.5f);
    rotationMatrixX.m[2][2] = cosf(fTheta * 0.5f);
    rotationMatrixX.m[3][3] = 1;

    for(auto tri : cubeMesh.triangles){
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        MultiplyMatrixVector(tri.points[0], triRotatedZ.points[0], rotationMatrixZ);
        MultiplyMatrixVector(tri.points[1], triRotatedZ.points[1], rotationMatrixZ);
        MultiplyMatrixVector(tri.points[2], triRotatedZ.points[2], rotationMatrixZ);
        
        MultiplyMatrixVector(triRotatedZ.points[0], triRotatedZX.points[0], rotationMatrixX);
        MultiplyMatrixVector(triRotatedZ.points[1], triRotatedZX.points[1], rotationMatrixX);
        MultiplyMatrixVector(triRotatedZ.points[2], triRotatedZX.points[2], rotationMatrixX);

        triTranslated = triRotatedZX;
        triTranslated.points[0].z = triRotatedZX.points[0].z + 3.0f;
        triTranslated.points[1].z = triRotatedZX.points[1].z + 3.0f;
        triTranslated.points[2].z = triRotatedZX.points[2].z + 3.0f;

        MultiplyMatrixVector(triTranslated.points[0], triProjected.points[0], projectionMatrix);
        MultiplyMatrixVector(triTranslated.points[1], triProjected.points[1], projectionMatrix);
        MultiplyMatrixVector(triTranslated.points[2], triProjected.points[2], projectionMatrix);

        triProjected.points[0].x += 1.0f; triProjected.points[0].y += 1.0f;
        triProjected.points[1].x += 1.0f; triProjected.points[1].y += 1.0f;
        triProjected.points[2].x += 1.0f; triProjected.points[2].y += 1.0f;

        triProjected.points[0].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[0].y *= (float)constants::SCREEN_HEIGHT / 2.0f;
        triProjected.points[1].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[1].y *= (float)constants::SCREEN_HEIGHT / 2.0f;
        triProjected.points[2].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[2].y *= (float)constants::SCREEN_HEIGHT / 2.0f;

        drawTriangle(triProjected);
    }

    SDL_RenderPresent(gRenderer);
}