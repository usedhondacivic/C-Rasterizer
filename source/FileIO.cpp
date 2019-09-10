#include <fstream>
#include <strstream>
#include <string>
#include <vector>

#include "Engine3D.h"


bool LoadFromObjectFile(std::string sFilename){
    std::ifstream f(sFilename);
    if(!f.is_open()){
        return false;
    }

    std::vector<vec3d> verts;

    return true;
}