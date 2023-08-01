//#include "stdlib.h"
//#include "stdio.h"
#include <string>
//#include "math.h"
//#include "vector"
//#include "ctype.h"
#include "SDL_ttf.h"

#include "SDL.h"
#include "SDL_image.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//#define SDL_main main

int x, y, dx, dy;
SDL_Texture *texture;
SDL_Rect dest;

SDL_Renderer *renderer;
SDL_Window *window;

/*
class Entity
{
private:
    int x, y;
    SDL_Texture *texture;
    SDL_Rect dest;

public:
    int dx, dy;

    Entity(){
        x = 0; y = 80; dx = 0; dy = 0;
    }

    ~Entity(){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    
    void loadTexture(std::string name){
        SDL_Surface *surface;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", name.std::string::c_str());
        surface = IMG_Load(name.std::string::c_str());
        texture = SDL_CreateTextureFromSurface(app.renderer, surface);
        SDL_FreeSurface(surface);
    }

    void blit(){
        dest.x = x;
        dest.y = y;

        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	    SDL_RenderCopy(app.renderer, texture, NULL, &dest);

    }
    void move(){
        x += dx;
        y += dy;
        if(x > SCREEN_WIDTH) x=SCREEN_WIDTH;
        if(x < 0) x=0;
    }
};*/

/*class Player: public virtual Entity {
    public:
        Player(){
            loadTexture(IMG_player);
        };
        ~Player(){};
};*/


bool running;  // Flag of main cycle work

void initSDL()
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;
	
	windowFlags = 0;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

	window = SDL_CreateWindow("Try logo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    x=0; y=0; dx=2; dy=2;

    running = true;
}

void cleanup(void)
{
	SDL_DestroyRenderer(renderer);
	
	SDL_DestroyWindow(window);
	
	SDL_Quit();
}

// Main function
int main(int argv, char** args)
{
	initSDL();

    SDL_Surface *surface;
    //SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", name.std::string::c_str());
    surface = IMG_Load("logo.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


	while (running)
	{
		SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
	    SDL_RenderClear(renderer);

        SDL_Event event;
        
        while( SDL_PollEvent(&event) != 0 ){
            if(event.type == SDL_QUIT) running = false;
        }

        dest.x += dx;
        dest.y += dy;

        if(dest.x+dest.w > SCREEN_WIDTH || dest.x < 0) dx = -dx;
        if(dest.y+dest.h > SCREEN_HEIGHT || dest.y < 0) dy = -dy;

        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	    SDL_RenderCopy(renderer, texture, NULL, &dest);

		SDL_RenderPresent(renderer);

		SDL_Delay(16);
	}
    
    cleanup();

	return 0;
}