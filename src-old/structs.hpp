// Base in-game global structs
#pragma once

#include "include.hpp"
#include "dataLoader.hpp"
#include "entity.hpp"


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
