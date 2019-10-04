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

matrix4x4 projectionMatrix;
matrix4x4 rotationMatrixX, rotationMatrixZ;
matrix4x4 translationMatrix;
matrix4x4 worldMatrix;

float fTheta;

void MultiplyMatrixVector(vec3d &input, vec3d &output, matrix4x4 &matrix){
    output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
    output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
    output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
    float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];
    if(w != 0.0f){
        output /= w;
    }
}

vec3d GetColor(float &dot){
    vec3d color = {dot * 255.0f, dot * 255.0f, dot * 255.0f};
    return color;
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

    std::vector<triangle> vecTrianglesToRaster;

    for(auto tri : cubeMesh.triangles){
        triangle triProjected, triTransformed;

        triTransformed.points[0] = tri.points[0] * worldMatrix;
        triTransformed.points[1] = tri.points[1] * worldMatrix;
        triTransformed.points[2] = tri.points[2] * worldMatrix;

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

            triProjected.points[0] = triTransformed.points[0] * projectionMatrix;
            triProjected.points[1] = triTransformed.points[1] * projectionMatrix;
            triProjected.points[2] = triTransformed.points[2] * projectionMatrix;

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
            drawTriangle(triProjected, triProjected.color);
            //std::cout << "X: " << triProjected.points[0].x << " Y: " << triProjected.points[0].y << " Z: " << triProjected.points[0].z << "\n";
            
        }
        
    }

    SDL_RenderPresent(gRenderer);
}