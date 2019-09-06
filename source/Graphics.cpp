#include <SDL2/SDL.h>

#include "Main.h"
#include "Engine3D.h"
#include "Constants.h"
#include "Graphics.h"

void drawTriangle(triangle &tri){
    SDL_RenderDrawLine( gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[2].x, (int)tri.points[2].y, (int)tri.points[0].x, (int)tri.points[0].y);
    thickLineColor(gRenderer, 0, 0, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT, 20, 0xFF00FFFF) ;
    thickLineColor(gRenderer, 0, constants::SCREEN_HEIGHT, constants::SCREEN_WIDTH, 0, 20, 0xFF00FFFF) ;
    circleColor(gRenderer, constants::SCREEN_WIDTH/2, constants::SCREEN_HEIGHT/2, 33, 0xff00ff00);
    filledCircleColor(gRenderer, constants::SCREEN_WIDTH/2, constants::SCREEN_HEIGHT/2, 30, 0xff00ffcc);
}