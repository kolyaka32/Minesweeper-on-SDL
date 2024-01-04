#pragma once

#include "baseGUI.hpp"
#include "structs.hpp"

// Working with text
// Functions for setting text at his place and his text
void setAllText();  
// Function of updating translation of all texts
void updateTranslation(LNG_types language);
// Function of clearing text SELCTED_BOX_types
void clearAllText();

// Pause menu with settings
void pause();

// Menu with end of game (mostly for screenshoots)
void endMenu();

// First loaded menu and game start waiting screen
void startMenu();

// Types of numerated static texts
enum TXT_types{
    TXT_START_KEYS,

    TXT_MINE_REST,
    TXT_TIME,

    TXT_END_WIN,
    TXT_END_LOOSE,
    TXT_END_SKIP,
    TXT_END_KEYS,

    TXT_PAUSE_PAUSE,
    TXT_PAUSE_MUSIC,
    TXT_PAUSE_SOUND
};
#define TXT_count 10

// Declaring static text
#if TXT_count
extern GUI::staticText* texts[TXT_count];
#endif

// Declaring global list of animations
#if ANI_count
extern Animation Advertisment[ANI_count];
#endif

extern GUI::Button* esc;

// External values to save and load from settings file
extern LNG_types language;  // Language settings
extern Uint8 MusicVolume;  // Start volume of music
extern Uint8 EffectsVolume;  // Start volume of effects
extern Uint16 drawFPS;  // Setted frames per seconds
