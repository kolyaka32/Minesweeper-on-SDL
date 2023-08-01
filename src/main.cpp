#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define mineX 16
#define mineY 20

#define mineCount 40  // Quantity of mines

#define mineSide 32  // Width and height of mine

#define HEADLINE 100  // Upper line for text

#define SCREEN_WIDTH mineX*mineSide
#define SCREEN_HEIGHT mineY*mineSide + HEADLINE

// Names of images in array to using
enum IMG_names{
    mine_close,
    mine_black,
    mine_green,
    mine_red,
    flag_normal,
    flag_wrong,
    number_0,
    number_1,
    number_2,
    number_3,
    number_4,
    number_5,
    number_6,
    number_7,
    number_8,
    IMG_count
};

// Names of files to load textures
std::string IMG_files[IMG_count]{
    "img/but.png",
    "img/but10.png",
    "img/but12.png",
    "img/but11.png",
    "img/but0.png",
    "img/but1.png",
    "img/num_0.png",
    "img/num_1.png",
    "img/num_2.png",
    "img/num_3.png",
    "img/num_4.png",
    "img/num_5.png",
    "img/num_6.png",
    "img/num_7.png",
    "img/num_8.png"
};

const char aroundPos[]{
    0, 1,
    1, 1,
    1, 0,
    1, -1,
    0, -1,
    -1, -1,
    -1, 0,
    -1, 1
};

struct App{
    SDL_Renderer *renderer;
    SDL_Window *window;
};

// Flags of running
bool running;  // Flag of main cycle work
bool loosing;  // Flag of showing loosing screen
bool winning;  // Flag of showing winning screen
bool firstClick;  // Flag of first click on screen to generate field

// In-game flags
bool Lclick;  // Flag of left click on screen
bool Rclick;  // Flag of right click on screen
char flagSet;  // Flag of non-stated/setting/unsetting flags on screen

// Global numbers
int flagedMines;  // number of marked mines
int realOpenMines;  // number of right choosen mines
int StartTimer;  // Start clock time 

// Texts variables and constants
TTF_Font* Font;
std::string TimerText;  // Text of timer
SDL_Texture *Textures[IMG_count];  // Array of all textures

void field_generator(const int number, const int WIDTH, const int HEIGHT, int firstX, int firstY, char* minesField){
    int* minesToChose = new int[WIDTH*HEIGHT];
    int k = 0;
    for(int x=0; x < WIDTH; ++x)
        for(int y=0; y < HEIGHT; ++y){
            if( (abs(firstX - x) > 1) || (abs(firstY - y) > 1) ){
                minesToChose[k] = ( x + y*WIDTH ); k++;
            };
    }
    for(int i=0; i < (WIDTH*HEIGHT); ++i){
        minesField[i] = 0;
    }
    
    for(int n=0; n < number; ++n){
        int chose = rand() % (k-n);
        unsigned char x = minesToChose[ chose ] % WIDTH;
        unsigned char y = minesToChose[ chose ] / WIDTH;
        for(int i = 0; i < 8; ++i){
            if((x + aroundPos[i * 2] >= 0) && (x + aroundPos[i * 2] < WIDTH) 
            && (y + aroundPos[i * 2 + 1] >= 0) && (y + aroundPos[i * 2 + 1] < HEIGHT)){
                int p = (aroundPos[i * 2] + x) + (aroundPos[i * 2 + 1] + y)*WIDTH;
                if(minesField[p] != -1){
                    minesField[p] += 1;
                }
            }
        }
        minesField[ y * WIDTH + x ] = -1;
        minesToChose[ chose ] = minesToChose[ k - n ];
    };

    delete minesToChose;
};

// Creating objects
App app;

void loadTextures(){
    SDL_Surface *temp_surface;
    for(int i = 0; i < IMG_count; ++i){
        //SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", IMG_files[i]);
        temp_surface = IMG_Load(IMG_files[i].std::string::c_str());
        Textures[i] = SDL_CreateTextureFromSurface(app.renderer, temp_surface);
        SDL_FreeSurface(temp_surface);
    }
};

void unloadTextures(){
    for(int i = 0; i < IMG_count; ++i){
        SDL_DestroyTexture(Textures[i]);
        Textures[i] = NULL;
    }
}

void drawText(std::string text, int x, int y){
    SDL_Surface *textSurface = TTF_RenderText_Solid(Font, text.std::string::c_str(), {255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app.renderer, textSurface);
    SDL_Rect textRect; textRect.x = x; textRect.y = y;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(app.renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

class Mine
{
private:
    int x, y;
    char state;
    SDL_Texture *texture;
    SDL_Rect dest;

public:
    Mine(){}
    ~Mine(){}

    void setPosition(int Posx, int Posy){
        x = Posx; y = Posy;
        dest.x = mineSide * Posx;
        dest.y = mineSide * Posy + HEADLINE;
    }

    void setState(char Newstate){
        state = Newstate;
    }
    
    void setTexture(SDL_Texture *Newtexture){
        texture = Newtexture;
    }

    void blit(){
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	    SDL_RenderCopy(app.renderer, texture, NULL, &dest); 
    }

    // State interpretetions:
    //    -1: mine; 0-8: cell with number; 10-18: open non-mine; 19: exploded mine; 20: flaged mine; 21-29: wrong flaged cell
    void Lclick();  // Function of activation cell
    void Rclick();  // Function of Flagging cell
    void actAround();  // Function of testing around cell to try activate them
    void open();  // Final function of showing internal state of cell
};

Mine field[mineX*mineY];  // Creating mines like objects

void Mine::Lclick(){
    if(state == -1){  // exploding mine
        texture = Textures[mine_red];
        state = 19;
        loosing = true;
    }
    else if(state < 9){  // Open curent mine
        texture = Textures[number_0 + state];
        // Open around cells, if it secure
        state += 10;  // Setting state of cell to open 
        if(state == 10){
            for(int i=0; i<8; ++i){
                if( ((aroundPos[i*2] + x) >= 0) && ((aroundPos[i*2] + x) < mineX) && ((aroundPos[i*2+1] + y) >= 0) && ((aroundPos[i*2+1] + y) < mineY) ){
                    field[ (aroundPos[i*2] + x) + (aroundPos[i*2+1] + y) * mineX ].Lclick();
                }
            }
        }
    }
}

void Mine::Rclick(){
    // Setting flag on cell
    if((state < 9) && (flagSet != 2)){  // Setting flag on curent cell
        texture = Textures[flag_normal];
        state += 21;
        if(state == 20){
            realOpenMines += 1;  // Counter of real mines
        }
        flagedMines += 1;
        if(flagSet == 0){
            flagSet = 1;
        }
    }
    else if((state > 19) && (flagSet != 1)){  // Unsetting flag on cell
        texture = Textures[mine_close];
        state -= 21;
        if(state == -1){
            realOpenMines -= 1;
        }
        flagedMines -= 1;
        if(flagSet == 0){
            flagSet = 2;
        }
    }
}

void Mine::actAround(){
    if(state > 9 && state < 19){
        int around = 0;
        for(int i=0; i<8; ++i){
            if( ((aroundPos[i*2] + x) >= 0) && ((aroundPos[i*2] + x) < mineX) && ((aroundPos[i*2+1] + y) >= 0) && ((aroundPos[i*2+1] + y) < mineY) ){
                if( field[ (aroundPos[i*2] + x) + (aroundPos[i*2+1] + y) * mineX ].state > 19 )
                    {around +=1;}
            }
        }
        if(state == around+10 && state != 10){
            for(int i=0; i<8; ++i){
                if( ((aroundPos[i*2] + x) >= 0) && ((aroundPos[i*2] + x) < mineX) && ((aroundPos[i*2+1] + y) >= 0) && ((aroundPos[i*2+1] + y) < mineY) ){
                    field[ (aroundPos[i*2] + x) + (aroundPos[i*2+1] + y) * mineX ].Lclick();
                }
            }
        }
    }
}

void Mine::open(){
    if(state == -1){
        texture = Textures[mine_black];
    }
    else if(state < 9){
        texture = Textures[number_0 + state];
    }
    else if(state == 19){
        texture = Textures[mine_red];
    }
    else if(state == 20){
        texture = Textures[mine_green];
    }
    else if(state > 20){
        texture = Textures[flag_wrong];
    }
}

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);  // Initialising SDL libarary

	app.window = SDL_CreateWindow("Try minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);  // Initialasing image library
    TTF_Init();  // Initialasing fonts library

    Font = TTF_OpenFont("Arial.ttf", 30);  // Reading font to drawing text

    // Resetting all variables
    running = true;
    firstClick = true;
    flagedMines = 0;
    realOpenMines = 0;
    loosing = false;
    winning = false;
    Lclick = false;
    Rclick = false;
}

void cleanup(void)
{
    TTF_CloseFont(Font);

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	
	SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

// Main function
int main(int argv, char** args)
{
    initSDL();  // Initialasing of main SDL library
    loadTextures();  // Loading sprites to the game

    char* mineField = new char[mineX*mineY];  // Creating array of states of mines
    for(int x = 0; x < mineX; ++x)  // Setting cell on positions
        for(int y=0; y < mineY; ++y){
            field[y*mineX + x].setPosition( x, y );
            field[y*mineX + x].setTexture( Textures[mine_close] );
        };

    SDL_Event event;
    // Main game cycle
	while(running)
	{
        // Clearing screen
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        while( SDL_PollEvent(&event) != 0 ){  // Getting events
            if(event.type == SDL_QUIT){
                running = false;  // Exit from program
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    Lclick = true;  // Activating chosen tile
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    Rclick = true;  // Setting flag
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    Lclick = false;
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    Rclick = false;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                // Resseting field and next new generation
                if (event.key.keysym.sym == SDLK_r) {
                    firstClick = true;  
                    for(int i=0; i < mineX*mineY; ++i){
                        field[i].setTexture( Textures[mine_close] );
                    }
                }
            }
        }

        // Getting mouse position on screen
        int MouseX, MouseY;
        SDL_GetMouseState(&MouseX, &MouseY);

        // ACtivating selected cell
        if((MouseY > HEADLINE) ){  //&& (MouseX > 0) && (MouseX < SCREEN_WIDTH)
            if(Lclick){
                if(firstClick){
                    // Creating test random field of mines
                    field_generator(mineCount, mineX, mineY, (MouseX / mineSide) , ((MouseY - HEADLINE) / mineSide), mineField);
                    for(int i = 0; i < mineX * mineY; ++i){
                        field[i].setState( mineField[i] );
                    }
                    
                    firstClick = false;
                    StartTimer = SDL_GetTicks();
                }
                field[(MouseX / mineSide) + ((MouseY - HEADLINE) / mineSide) * mineX].Lclick();
                field[(MouseX / mineSide) + ((MouseY - HEADLINE) / mineSide) * mineX].actAround();
            }
            if(Rclick){
                field[(MouseX / mineSide) + ((MouseY - HEADLINE) / mineSide) * mineX].Rclick();
            }
            else{
                flagSet = 0;  // Resetting flag of type of setting
            }
        }
        if(realOpenMines == mineCount && mineCount == flagedMines){
            winning = true;  // Activating winning of the game
        }

        // Drawing mines at screen
        for(int i = 0; i < mineX*mineY; ++i){
            field[i].blit();
        };

        // Creating text to drawing
        if(firstClick){
            TimerText = "0,00";
        }
        else{
            TimerText = std::to_string( (SDL_GetTicks() - StartTimer)/1000 ) + ',' 
            + std::to_string( (SDL_GetTicks()-StartTimer)/100%10 ) + std::to_string( (SDL_GetTicks()-StartTimer)/10%10 );
        }

        // Drawing text at screen
        drawText( std::to_string(mineCount - flagedMines), 20 , 15);
        drawText( TimerText, SCREEN_WIDTH - 100 , 15);

        // Blitting textures on screen
		SDL_RenderPresent(app.renderer);

        // Showing ending screen, if nececery
        if(winning || loosing){
            // Showing extra text
            SDL_Surface *extraSurface;
            if(winning){
                drawText("You win!", SCREEN_WIDTH/2 - 60, 15);
            }
            else if(loosing){
                drawText("You lose!", SCREEN_WIDTH/2 - 60, 15);
            }
            drawText( "Press 'r' to restart", SCREEN_WIDTH/2 - 120 , 60);

            // Openning all mines for understanding problems
            for(int i=0; i<mineX*mineY; ++i){
                field[i].open();
                field[i].blit();
            }
            SDL_RenderPresent(app.renderer);

            // Starting loop for waiting for start
            bool waiting = true;
            while(waiting){
                while( SDL_PollEvent(&event) != 0 ){
                    if(event.type == SDL_QUIT){
                        running = false;  // Exit from program
                        waiting = false;
                    }
                    if (event.key.keysym.sym == SDLK_r) {
                        for(int i=0; i < mineX*mineY; ++i){
                            field[i].setTexture( Textures[mine_close] );
                            field[i].blit();
                        }
                        waiting = false;
                    }
                }
                SDL_Delay(16);
            }
            SDL_RenderPresent(app.renderer);
            // Resetting values
            winning = false;
            loosing = false;
            firstClick = true;
            Lclick = false;
            Rclick = false;
            flagedMines = 0;
            realOpenMines = 0;
            }
		SDL_Delay(16);
	}

    // Cleaning all data
    delete [] mineField;
    unloadTextures();
    cleanup();
	return 0;
}