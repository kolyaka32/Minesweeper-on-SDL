#pragma once

#include "baseGUI.hpp"
#include "structs.hpp"



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
extern GUI::staticText texts[TXT_count];
#endif


extern GUI::Button* esc;
