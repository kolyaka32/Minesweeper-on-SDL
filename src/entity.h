#pragma once

#include "include.h"

// Game classes
// Main "mine" class
typedef struct{
    mineState state;
    SDL_Texture *texture;
    SDL_Rect dest;
} Mine;

void mineInit(Mine* mine, coord X, coord Y);       // Function of setting mine at his need position
void mineLMBclick(Mine* mine, coord X, coord Y);   // Function of activation cell
void mineFlagSet(Mine* mine);                      // Function of flagging cell
void mineFlagReset(Mine* mine);                    // Function of unflagging cell
void mineActAround(Mine* mine, coord X, coord Y);  // Function of testing around cell to try activate them

// Field class - useful structure for better use with mines
typedef struct{
    Mine* mines;
    Uint8 flagSetting;
} Field;

void newField();
void freeField(Field* f);

// Generating new field of mines with random positions
void fieldGenerate(const int number, const int WIDTH, const int HEIGHT, coord firstX, coord firstY);  
void fieldReset();  // Resetting states and textures of all mines to basic

void fieldLMBClick(const coord x, const coord y);     // Activate current mine and near none-zero mines
void fieldRMBClick(const coord x, const coord y);     // Set/unset flag on field
void fieldActAround(const coord x, const coord y);  // Recurcive activating near mines, if on cell equal to flag number
void fieldRMBUnpress();                               // Deactivating right clicking
int fieldGetState(const coord x, const coord y);    // Getting state of need cell
void fieldOpeanAll();                               // Openning all cell to show their states
void fieldBlit();                                   // Drawing all mines
