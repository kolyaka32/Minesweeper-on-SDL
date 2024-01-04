#pragma once

#include "include.hpp"

// Game classes
// Main "mine" class
class Mine
{
private:
    friend class Field;
    mineState state;
    SDL_Texture *texture;
    SDL_Rect dest;

public:
    void init(coord X, coord Y);       // Function of setting mine at his need position
    void blit();                       // Function of drawing need mine at screen
    void Lclick(coord X, coord Y);     // Function of activation cell
    void FlagSet();                    // Function of flagging cell
    void FlagReset();                  // Function of unflagging cell
    void actAround(coord X, coord Y);  // Function of testing around cell to try activate them
    void open();                       // Final function of showing internal state of cell
};

// Field class - useful structure for better use with mines
class Field
{
private:
    Mine* mines;
    Uint8 flagSetting;
public:
    Field();  // Basic constructor with setting memory for it
    ~Field();  // Clearing momory after all
    // Generating new field of mines with random positions
    void generateField(const int number, const int WIDTH, const int HEIGHT, coord firstX, coord firstY);  
    void reset();  // Resetting states and textures of all mines to basic

    void LClick(const coord x, const coord y);     // Activate current mine and near none-zero mines
    void RClick(const coord x, const coord y);     // Set/unset flag on field
    void actAround(const coord x, const coord y);  // Recurcive activating near mines, if on cell equal to flag number
    void RUnpress();                               // Deactivating right clicking
    int getState(const coord x, const coord y);    // Getting state of need cell
    void opeanAll();                               // Openning all cell to show their states
    void blit();                                   // Drawing all mines
};
