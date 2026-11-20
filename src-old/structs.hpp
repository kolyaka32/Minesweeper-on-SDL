// Base in-game global structs
#pragma once

#include "include.hpp"
#include "dataLoader.hpp"
#include "entity.hpp"

struct App{
    SDL_Renderer *renderer;
    SDL_Window *window;
};

// Data for creating fonts
#if FNT_count
extern Uint8* fontMemory;  // Memory with font data
extern uint64_t fontSize;  // Size of memory buffer
#endif

// Creating main varables
extern App app;  // Global main render and window

// All textures
#if IMG_count
extern SDL_Texture* Textures[IMG_count];  // Array of all textures
#endif

// All animations
#if ANI_count
extern IMG_Animation* Animations[ANI_count];  // Array of all animations
#endif

// All music trcks
#if MUS_count
extern Mix_Music* Musics[MUS_count];      // Array of all music
#if ARCHIEVE_LOADING
extern SDL_RWops* MusicsData[MUS_count];  // Array of data for music
#endif
#endif

// All effects sounds
#if SND_count
extern Mix_Chunk* Sounds[SND_count];  // Array of all sound effects
#endif

// Flags of running
extern bool running;  // Flag of main cycle work
extern bool loosing;  // Flag of showing loosing screen
extern bool winning;  // Flag of showing winning screen
extern bool skipping;  // Flag of showing screen of skipping 

// Counters from initialase file
extern coord mineX;          // Width of field in mines
extern coord mineY;          // Height of field in mines
extern count mineCount;      // Counter of number of mines at field
extern count leftTimer;      // Counter of time for fully solve

// Global counters
extern count flagedMines;    // Number of marked mines
extern count realOpenMines;  // Number of right choosen mines
extern Uint64 screenTimer;   // Timer of seconds since game start

// Global entities
extern Field* field;
