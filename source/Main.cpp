#include <SDL2/SDL.h>
#include <string>
#include <iostream>

#include "Engine3D.h"
#include "Main.h"
#include "Constants.h"

bool init();
void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

class LTimer
{
    public:
		LTimer(){
            mStartTicks = 0;
            mPausedTicks = 0;

            mPaused = false;
            mStarted = false;
        };

		void start(){
            mStarted = true;
            mPaused = false;
            mStartTicks = SDL_GetTicks();
            mPausedTicks = 0;
        };

		void stop(){
            mStarted = false;
            mPaused = false;
            mStartTicks = 0;
            mPausedTicks = 0;
        };

		void pause(){
            if( mStarted && !mPaused )
            {
                mPaused = true;
                mPausedTicks = SDL_GetTicks() - mStartTicks;
                mStartTicks = 0;
            }
        };

		void unpause(){
            if( mStarted && mPaused )
            {
                mPaused = false;
                mStartTicks = SDL_GetTicks() - mPausedTicks;
                mPausedTicks = 0;
            }
        };

		Uint32 getTicks(){
            Uint32 time = 0;
            if( mStarted ){
                if( mPaused ){
                    time = mPausedTicks;
                }else{
                    time = SDL_GetTicks() - mStartTicks;
                }
            }
            return time;
        };

		bool isStarted(){
            return mStarted;
        };

		bool isPaused(){
            return mPaused && mStarted;
        };

    private:
		Uint32 mStartTicks;

		Uint32 mPausedTicks;

		bool mPaused;
		bool mStarted;
};

bool init(){
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    }else{
        if(!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")){
            std::cout << "Warning: Opengl rendering is not enabled!";
        }

        gWindow = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        }else{
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                std::cout << "Renderer could not be created! SDL Error: " <<  SDL_GetError();
                success = false;
            }else{
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderSetLogicalSize(gRenderer, constants::LOGICAL_WIDTH, constants::LOGICAL_HEIGHT);
            }
        }
    }

    return success;
}

void close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();
}

int main(int argc, char* args[]){
    if(!init()){
        std::cout << "Failed to initialize!" << "\n";
    }else{
        if(!setup()){
            std::cout << "Failed to setup!" << "\n";
        }else{
            bool quit = false;

            SDL_Event e;

            LTimer fpsTimer;

            LTimer capTimer;

            int countedFrames = 0;
            fpsTimer.start();

            while(!quit){
                capTimer.start();
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT){
                        quit = true;
                    }else if(e.type == SDL_KEYDOWN){
                        onKeyDown(e.key.keysym.sym);
                    }else if(e.type == SDL_KEYUP){
                        onKeyUp(e.key.keysym.sym);
                    }
                }

                float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);
                if(avgFPS > 2000000){
                    avgFPS = 0;
                }

                std::string title = "Rasterizer - FPS: " + std::to_string(avgFPS);
                char *cTitle = new char[title.size() + 1];
                title.copy(cTitle, title.size() + 1);
                cTitle[title.size()] = '\0';

                SDL_SetWindowTitle(gWindow, cTitle);

                update();
                
                SDL_RenderPresent(gRenderer);

                countedFrames++;

                int frameTicks = capTimer.getTicks();
                if( frameTicks < constants::SCREEN_TICK_PER_FRAME )
                {
                    SDL_Delay( constants::SCREEN_TICK_PER_FRAME - frameTicks );
                }
            }
        }
    }
    
    return 0;
}