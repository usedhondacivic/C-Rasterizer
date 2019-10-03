#pragma once
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "Vectorlib/Vec3d.h"
#include "Vectorlib/Matrix4x4.h"

struct triangle
{
    vec3d points[3] = {{}};

    vec3d color;
};

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

bool setup();

void update();