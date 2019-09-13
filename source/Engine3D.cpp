#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"
#include "Graphics.h"

struct mesh
{
    std::vector<triangle> triangles;

    bool LoadFromObjectFile(std::string sFilename){
        std::ifstream f(sFilename);
        if(!f.is_open()){
            return false;
        }

        std::vector<vec3d> verts;

        while(!f.eof()){
            char line[128];
            f.getline(line, 128);

            std::stringstream s;
            s << line;

            char junk;

            if(line[0] == 'v'){
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if(line[0] == 'f'){
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                triangles.push_back({ verts[f[0] - 1],  verts[f[1] - 1], verts[f[2] - 1]});
            }
        }

        return true;
    }
};

struct matrix4x4
{
    float m[4][4] = {0};
};

auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

mesh cubeMesh;

vec3d vCamera;

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

vec3d GetColor(float &dot){
    vec3d color = {(dot + 1.0f) * 127.5f, (dot + 1.0f) * 127.5f, (dot + 1.0f) * 127.5f};
    return color;
}

bool setup(){
    /*cubeMesh.triangles = {
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
    };*/

    cubeMesh.LoadFromObjectFile("./models/ship.obj");

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

    fTheta += 1.0f * fElapsedTime;

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

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

    std::vector<triangle> vecTrianglesToRaster;

    for(auto tri : cubeMesh.triangles){
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        MultiplyMatrixVector(tri.points[0], triRotatedZ.points[0], rotationMatrixZ);
        MultiplyMatrixVector(tri.points[1], triRotatedZ.points[1], rotationMatrixZ);
        MultiplyMatrixVector(tri.points[2], triRotatedZ.points[2], rotationMatrixZ);
        
        MultiplyMatrixVector(triRotatedZ.points[0], triRotatedZX.points[0], rotationMatrixX);
        MultiplyMatrixVector(triRotatedZ.points[1], triRotatedZX.points[1], rotationMatrixX);
        MultiplyMatrixVector(triRotatedZ.points[2], triRotatedZX.points[2], rotationMatrixX);

        triTranslated = triRotatedZX;
        triTranslated.points[0].z = triRotatedZX.points[0].z + 15.0f;
        triTranslated.points[1].z = triRotatedZX.points[1].z + 15.0f;
        triTranslated.points[2].z = triRotatedZX.points[2].z + 15.0f;

        vec3d normal, line1, line2;

        line1.x = triTranslated.points[1].x - triTranslated.points[0].x;
        line1.y = triTranslated.points[1].y - triTranslated.points[0].y;
        line1.z = triTranslated.points[1].z - triTranslated.points[0].z;

        line2.x = triTranslated.points[2].x - triTranslated.points[0].x;
        line2.y = triTranslated.points[2].y - triTranslated.points[0].y;
        line2.z = triTranslated.points[2].z - triTranslated.points[0].z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= length; normal.y /= length; normal.z /= length;

        if(
            normal.x * (triTranslated.points[0].x - vCamera.x) + 
            normal.y * (triTranslated.points[0].y - vCamera.y) + 
            normal.z * (triTranslated.points[0].z - vCamera.z) < 0
        ){
            vec3d lightDirection = {0.0f, 0.0f, -1.0f};
            float length = sqrtf(lightDirection.x * lightDirection.x + lightDirection.y * lightDirection.y + lightDirection.z * lightDirection.z);
            lightDirection.x /= length; lightDirection.y /= length; lightDirection.z /= length;

            float dot = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;

            triProjected.color = GetColor(dot);

            MultiplyMatrixVector(triTranslated.points[0], triProjected.points[0], projectionMatrix);
            MultiplyMatrixVector(triTranslated.points[1], triProjected.points[1], projectionMatrix);
            MultiplyMatrixVector(triTranslated.points[2], triProjected.points[2], projectionMatrix);

            triProjected.points[0].x += 1.0f; triProjected.points[0].y += 1.0f;
            triProjected.points[1].x += 1.0f; triProjected.points[1].y += 1.0f;
            triProjected.points[2].x += 1.0f; triProjected.points[2].y += 1.0f;

            triProjected.points[0].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[0].y *= (float)constants::SCREEN_HEIGHT / 2.0f;
            triProjected.points[1].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[1].y *= (float)constants::SCREEN_HEIGHT / 2.0f;
            triProjected.points[2].x *= (float)constants::SCREEN_WIDTH / 2.0f; triProjected.points[2].y *= (float)constants::SCREEN_HEIGHT / 2.0f;

            vecTrianglesToRaster.push_back(triProjected);
        }

        std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2){
            float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
            float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;

            return z1 > z2;
        });

        for(auto &triProjected : vecTrianglesToRaster){
            drawTriangle(triProjected, triProjected.color);
        }
    }

    SDL_RenderPresent(gRenderer);
}