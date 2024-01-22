#pragma once

// Extra modifications for game
#define DEBUG false
#define ARCHIEVE_LOADING true
#define SCROLLER_SOUND SND_count
#define ADEVERTISMENT ANI_count

// System numbers
#define true 1                                  // System true varibale for bool
#define false 0                                 // System false varibale for bool
#define WINDOWNAME "Minesweeper on SDL"         // System game name
#define MINE_SIDE 32                            // Width and height of mine in pixels
#define GAME_WIDTH MINE_SIDE * mineX            // Width of game area
#define GAME_HEIGHT MINE_SIDE * mineY           // Height of game area
#define HEADLINE 128                            // Upper line for text
#define SCREEN_WIDTH (GAME_WIDTH)               // Width of all screen
#define SCREEN_HEIGHT (GAME_HEIGHT + HEADLINE)  // Height of all screen
// Base file names
#define SETTING_FILE "setting.ini"              // File with all starting data (width, height...)
#define DATA_FILE "data.dat"                    // File with all pictures, music...

// Base game colors
#define BLACK {0, 0, 0, 255}
#define WHITE {255, 255, 255, 255}
#define GREY {32, 32, 32, 255}
#define BACKGROUND_COLOR 0, 0, 0, 255

// Types of data
typedef uint8_t bool;       // Type of data for true/false
typedef uint8_t coord;      // Type of data for coordinates
typedef uint16_t count;     // Type of data for mines counters
typedef uint8_t mineState;  // Type of data for saving mine state (from MINE_STATES)

// Types of language
enum LNG_types{
    LNG_ENGLISH,  // English language
    LNG_RUSSIAN   // Russian language
};

#define LNG_count 2  // Final counter of all languages

// Types of mines 
enum MINE_STATES{
    MINE_NORMAL_0 = 0,
    MINE_NORMAL_8 = 8,
    MINE_MINE = 9,

    MINE_OPEN_NORMAL_0 = 10,
    MINE_OPEN_NORMAL_8 = 18,

    MINE_EXPLODED = 19,

    MINE_FLAG_WRONG_0 = 20,
    MINE_FLAG_WRONG_8 = 28,
    MINE_FLAG_CORRECT = 29
};

// Types of setting flag (on RCLICK)
enum FLAG_SETTING{
    FLAG_NONE,
    FLAG_SET,
    FLAG_RESET
};

// Macroses
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)