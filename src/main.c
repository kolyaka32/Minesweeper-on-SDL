
#include "include.h"
#include "define.h"
#include "structs.h"

#include "init.h"
#include "initFile.h"
#include "dataLoader.h"
#include "baseGUI.h"
#include "pause.h"
#include "entity.h"

// Creating main window variables
App app;  

// Global numbers
// Last time and previous ticks update
Uint64 oldMoveTime;
Uint64 oldDrawTime;
Uint16 lastBoostTicks;  // Ticks from boost activation

// Pause and init settings
enum LNG_types language;         // Switched languaged from language_types
Uint8 MusicVolume;      // Volume of music
Uint8 EffectsVolume;    // Volume of effects
Uint16 drawFPS;         // Max terget FPS to draw

// Counters and constants from initialase file
coord mineX;          // Width of field in mines
coord mineY;          // Height of field in mines
count mineCount;      // Counter of number of mines at field
count leftTimer;      // Counter of time for fully solve
// Global counters
count flagedMines;    // Number of marked mines
count realOpenMines;  // Number of right choosen mines
Uint64 screenTimer;   // Timer of seconds sincs game start

// Global entities
Field* field;

// Global running flags
bool running = true;   // Flag of main cycle work
bool loosing;   // Flag of showing loosing screen
bool winning;   // Flag of showing winning screen
bool skipping;  // Flag of showing screen of skipping 

// Texts variables and constants
#if IMG_count
SDL_Texture* Textures[IMG_count];  // Array of all textures
#endif
#if ANI_count
IMG_Animation* Animations[ANI_count];  // Array of all animations
#endif
#if MUS_count
Mix_Music* Musics[MUS_count];      // Array of all music
#if ARCHIEVE_LOADING
SDL_RWops* MusicsData[MUS_count];  // Array of data for music
#endif
#endif
#if SND_count
Mix_Chunk* Sounds[SND_count];  // Array of all sound effects
#endif
#if FNT_count
Uint8* fontMemory;    // Memory with font data
uint64_t fontSize;    // Size of memory buffer
#endif

// Main function
int main(int argv, char** args){
    initLibraries();  // Initialasing all need libraries
    loadInitFile();   // Load initialasing file file with settings
    createVideo();    // Creating video output system
    
    // Loading data from file
    loadData();

    #if MUS_count
    // Setting volumes of sounds
    Mix_VolumeMusic(MusicVolume);  // Setting volume of music
    Mix_Volume(-1, EffectsVolume);  // Setting volume of effects
    // Infinite playing main game theme
    Mix_PlayMusic( Musics[MUS_MENU_THEME], -1 );  
    #endif

    // Creating user interface
    initStaticTexts();
    updateTranslation(language);
    initButton(&esc);

    // Creating mine field and initialasing it
    newField();

    // Showing enter screen and waiting for start
    startMenu();

    // Cycle variables
    bool LClicking = false;
    bool RClicking = false;
    SDL_Event event;

	while(running)  // Main game cycle
	{
        // Showing waiting start menu screen
        if(loosing || winning || skipping){
            endMenu();
            startMenu();
        }

        // Getting events
        while( SDL_PollEvent(&event) != 0 ){  
            switch (event.type){
            case SDL_QUIT:
                running = false;  // Exit from program
                break;

            case SDL_KEYDOWN:
                // Resseting field and next new generation
                switch (event.key.keysym.sym)
                {
                case SDLK_r:
                    skipping = true;
                    break;
                
                case SDLK_ESCAPE:
                    pause();
                    LClicking = false;
                    RClicking = false;
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    LClicking = true;

                    Mix_PlayChannel(-1, Sounds[SND_CLICK], 0);  // Playing need sound
                    break;
                case SDL_BUTTON_RIGHT:
                    RClicking = true;
                    
                    Mix_PlayChannel(-1, Sounds[SND_FLAG_SET], 0);  // Playing need sound
                    break;
                }
                break;
            
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    LClicking = false;
                    break;
                
                case SDL_BUTTON_RIGHT:
                    RClicking = false;
                    fieldRMBUnpress();
                    break;
                }
            }
        }

        // Updating button pressing
        int MouseX, MouseY;
        // Getting mouse position on screen
        SDL_GetMouseState(&MouseX, &MouseY);

        // ACtivating selected cell
        if((MouseY > HEADLINE) ){
            if(LClicking){
                fieldLMBClick(MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
                fieldActAround(MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
            }
            if(RClicking){
                fieldRMBClick(MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
            }
        }
        else{
            if(LClicking && checkInButton(&esc, MouseX, MouseY)){
                pause();
                LClicking = false;
                RClicking = false;
            }
        }

        // Activating winning of the game
        if(realOpenMines == mineCount && flagedMines == mineCount){
            winning = true;
            Mix_PlayChannel(-1, Sounds[SND_WIN], 0);
        }

        // Checking, if drawing need
        if(SDL_GetTicks64() - oldDrawTime > 1000 / drawFPS){
            // Updating dinamic texts
            updateStaticText(texts + TXT_MINE_REST, mineCount - flagedMines);
            // Updating timer value, depend, if not equal 0 - rest time
            if(leftTimer){
                updateStaticText(texts + TXT_TIME, leftTimer - (SDL_GetTicks64() - screenTimer) / 1000);

                // Checking, if timer end - loosing game
                if((SDL_GetTicks64() - screenTimer) / 1000 >= leftTimer){
                    loosing = true;
                    Mix_PlayChannel(-1, Sounds[SND_LOOSE], 0);
                }
            }
            else{
                updateStaticText(texts + TXT_TIME, (SDL_GetTicks64() - screenTimer) / 1000);
            }

            // Clearing screen
            SDL_RenderClear(app.renderer);

            // Drawing main game
            fieldBlit();

            // Drawing interface
            blitStaticText(texts + TXT_MINE_REST);
            blitStaticText(texts + TXT_TIME);
            blitButton(&esc);

            // Blitting textures on screen
            SDL_RenderPresent(app.renderer);

            oldDrawTime = SDL_GetTicks64();  // Getting last update time
        };

        // Waiting until next moving or drawing
        if(SDL_GetTicks64() > oldDrawTime + 1000/drawFPS){
            SDL_Delay( SDL_GetTicks64() - oldDrawTime - 1000/drawFPS );
        }
	}
    // Exiting program
    // Saving all data to setting file for next starts
    saveInitFile();  

    // Clearing static texts
    #if TXT_count
    freeStaticTexts();
    #endif

    // Clearing dinamic texts
    freeField(field);

    // Cleaning all data
    unloadData();

    // Exiting
    deleteVideo();  // Destroying video output
    exitLibraries();  // Exiting all libraries
	return 0;
}