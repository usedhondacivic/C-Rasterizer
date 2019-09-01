#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Main.h"
#include "Engine3D.h"
#include "Constants.h"
#include "Graphics.h"

void drawTriangle(triangle &tri){
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
    SDL_RenderDrawLine( gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[2].x, (int)tri.points[2].y, (int)tri.points[0].x, (int)tri.points[0].y);
}