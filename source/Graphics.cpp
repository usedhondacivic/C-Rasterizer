#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Main.h"
#include "Engine3D.h"
#include "Constants.h"

void drawTriangle(triangle tri){
    SDL_RenderDrawLine( gRenderer, 0, constants::SCREEN_HEIGHT / 2, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT / 2 );
}