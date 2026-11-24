#pragma once


// System numbers
#define WINDOWNAME "Minesweeper on SDL"         // System game name
#define MINE_SIDE 32                            // Width and height of mine in pixels
#define GAME_WIDTH MINE_SIDE * mineX            // Width of game area
#define GAME_HEIGHT MINE_SIDE * mineY           // Height of game area
#define HEADLINE 128                            // Upper line for text
#define SCREEN_WIDTH (GAME_WIDTH)               // Width of all screen
#define SCREEN_HEIGHT (GAME_HEIGHT + HEADLINE)  // Height of all screen
// Base file names
#define SETTING_FILE "settings2.ini"            // File with all starting data (width, height...)
#define DATA_FILE "data2.dat"                   // File with all pictures, music...



// Types of data

typedef uint16_t count;     // Type of data for mines counters
typedef uint8_t mineState;  // Type of data for saving mine state (from MINE_STATES)




// Array of positions, near procedral cell
const Sint8 aroundPos[]{
    0, 1,
    1, 1,
    1, 0,
    1, -1,
    0, -1,
    -1, -1,
    -1, 0,
    -1, 1
};



// Types of setting flag (on RCLICK)
enum FLAG_SETTING{
    FLAG_NONE,
    FLAG_SET,
    FLAG_RESET
};
