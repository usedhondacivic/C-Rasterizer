#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Main.h"
#include "Engine3D.h"
#include "Constants.h"
#include "Graphics.h"

void drawTriangle(triangle &tri, vec3d &color){
    /*SDL_SetRenderDrawColor(gRenderer, color.x, color.y, color.z, 255);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[2].x, (int)tri.points[2].y, (int)tri.points[0].x, (int)tri.points[0].y);*/
    filledTrigonRGBA(gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y, color.x, color.y, color.z, 255);
    //trigonRGBA(gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y, color.x, color.y, color.z, 255);
}