#pragma once

#include "include.hpp"
#include "define.hpp"
#include "structs.hpp"

// Load file with all data
void loadData();

// Function of clearing all temporary data, loaded from arcieve
void unloadData();

// Archive values
#define PASSWORD NULL  // Archive password

// Used numerated numbers
#define ICO_count 1  // Number of icones for better count
#define FNT_count 1  // Number of fonts for better count

// Names of images in array to using
enum IMG_names{
    // Menu icones
    IMG_MENU_PAUSE,
    IMG_MENU_SCROLLER_BUTTON,
    IMG_MENU_SCROLLER_LINE,

    // Flags in settings
    IMG_FLAG_USA,
    IMG_FLAG_RUS,

    // Mines on field icones
    IMG_MINE_CLOSE,
    IMG_MINE_NORMAL,
    IMG_MINE_GREEN,
    IMG_MINE_RED,

    // Flags on field icones
    IMG_FLAG_RIGHT,
    IMG_FLAG_WRONG,

    // Open mines icones on field
    IMG_MINE_0,
    IMG_MINE_1,
    IMG_MINE_2,
    IMG_MINE_3,
    IMG_MINE_4,
    IMG_MINE_5,
    IMG_MINE_6,
    IMG_MINE_7,
    IMG_MINE_8
};
#define IMG_count 20

// Names of music in array to use
enum MUS_names{
    MUS_MAIN_THEME,
    MUS_MENU_THEME
};
#define MUS_count 2

// Names of sound effects in array to use
enum SND_names{
    SND_CLICK,
    SND_FLAG_SET,
    SND_LOOSE,
    SND_WIN,
};
#define SND_count 4

// Names of animations in array to use
enum ANI_names{

};
#define ANI_count 0
