#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"
#include "Graphics.h"

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

mesh cubeMesh;

vec3d vCamera;
vec3d vLookDirection;

matrix4x4 projectionMatrix;
matrix4x4 rotationMatrixX, rotationMatrixZ;
matrix4x4 translationMatrix;
matrix4x4 worldMatrix;
matrix4x4 cameraMatrix;
matrix4x4 viewMatrix;

float fTheta;

vec3d GetColor(float &dot){
    vec3d color = {dot * 255.0f, dot * 255.0f, dot * 255.0f};
    return color;
}

bool up, down, left, right = false;

void onKeyDown(SDL_Keycode key){
    if(key == SDLK_UP){
        up = true;
    }
    if(key == SDLK_DOWN){
        down = true;
    }
    if(key == SDLK_LEFT){
        left = true;
    }
    if(key == SDLK_RIGHT){
        right = true;
    }
}

void onKeyUp(SDL_Keycode key){
    if(key == SDLK_UP){
        up = false;
    }
    if(key == SDLK_DOWN){
        down = false;
    }
    if(key == SDLK_LEFT){
        left = false;
    }
    if(key == SDLK_RIGHT){
        right = false;
    }
}

bool setup(){
    cubeMesh.LoadFromObjectFile("./models/ship.obj");

    //Projection Matrix
    float fNear = 0.1f; 
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)constants::LOGICAL_HEIGHT / (float)constants::LOGICAL_WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    projectionMatrix.makeProjection(90.0f, (float)constants::LOGICAL_HEIGHT / (float)constants::LOGICAL_WIDTH, 0.1f, 1000.0f);

    return true;
}

void update(){
    if(up)
        vCamera.y += 8.0f;
    
    if(down)
        vCamera.y -= 8.0f;

    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    float fElapsedTime = elapsedTime.count();

    fTheta += 1.0f * fElapsedTime;

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    rotationMatrixZ.makeRotationZ(fTheta);
    rotationMatrixX.makeRotationX(fTheta);

    translationMatrix.makeTranslation(0.0f, 0.0f, 16.0f);

    worldMatrix.makeIdentity();
    worldMatrix = rotationMatrixZ * rotationMatrixX;
    worldMatrix = worldMatrix * translationMatrix;

    vLookDirection = {0, 0, 1};
    vec3d vUp = {0, 1, 0};
    vec3d vTarget = vCamera + vLookDirection;

    cameraMatrix.makePointAt(vCamera, vTarget, vUp);
    viewMatrix.makeQuickInverse(cameraMatrix);


    std::vector<triangle> vecTrianglesToRaster;

    for(auto tri : cubeMesh.triangles){
        triangle triProjected, triTransformed, triViewed;

        triTransformed.points[0] = worldMatrix * tri.points[0];
        triTransformed.points[1] = worldMatrix * tri.points[1];
        triTransformed.points[2] = worldMatrix * tri.points[2];

        vec3d normal, line1, line2;

        line1 = triTransformed.points[1] - triTransformed.points[0];
        line2 = triTransformed.points[2] - triTransformed.points[1];

        normal = line1.cross(line2);

        normal = normal.normal();

        vec3d vCameraRay = triTransformed.points[0] - vCamera;

        if(normal.dot(vCameraRay) < 0.0f){
            vec3d lightDirection = {0.0f, 0.0f, -1.0f};
            lightDirection = lightDirection.normal();

            float dot = std::max(0.1f, lightDirection.dot(normal));

            triProjected.color = GetColor(dot);

            triViewed.points[0] = viewMatrix * triTransformed.points[0];
            triViewed.points[1] = viewMatrix * triTransformed.points[1];
            triViewed.points[2] = viewMatrix * triTransformed.points[2];

            triProjected.points[0] = projectionMatrix * triViewed.points[0];
            triProjected.points[1] = projectionMatrix * triViewed.points[1];
            triProjected.points[2] = projectionMatrix * triViewed.points[2];

            triProjected.points[0] /= triProjected.points[0].w;
            triProjected.points[1] /= triProjected.points[1].w;
            triProjected.points[2] /= triProjected.points[2].w;

            vec3d vOffsetView = {1, 1, 0};
            triProjected.points[0] += vOffsetView;
            triProjected.points[1] += vOffsetView;
            triProjected.points[2] += vOffsetView;
            
            vec3d vScaleView = {(float)constants::LOGICAL_WIDTH / 2.0f, (float)constants::LOGICAL_HEIGHT / 2.0f, 1};
            triProjected.points[0] *= vScaleView;
            triProjected.points[1] *= vScaleView;
            triProjected.points[2] *= vScaleView;

            vecTrianglesToRaster.push_back(triProjected);
        }

        std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2){
            float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
            float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;

            return z1 > z2;
        });

        for(auto &triProjected : vecTrianglesToRaster){
            fillTriangle(triProjected, triProjected.color, true);
        }
        
    }
    /*for(int i = 0; i < 1000; i++){
        vec3d one = {0, 100, 200};
        vec3d two = {200, 350, 700};
        vec3d three = {150, 70, 1000};
        triangle test = {one, two, three};
        vec3d color = {255, 255, 255};
        fillTriangle(test, color);
    }*/
}