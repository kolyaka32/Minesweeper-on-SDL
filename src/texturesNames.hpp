/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "define.hpp"

#if (PRELOAD_TEXTURES)


// Names of all images with related numbers
enum class Textures : unsigned {
    // Base part
    // Graphic interface sprites
    SliderButton,
    SliderLine,
    //MenuButton,
    SettingsButton,

    // Base flags in settings
    FlagUSA,
    FlagRUS,

    // Mines on field icones
    MineClose,
    MineNormal,
    MineGreen,
    MineRed,

    // Flags on field icones
    FlagRight,
    FlagWrong,

    // Open mines icones on field
    Mine0,
    Mine1,
    Mine2,
    Mine3,
    Mine4,
    Mine5,
    Mine6,
    Mine7,
    Mine8,

    // Global counter of all textures
    Count,
};

// Operators for change textures depend on offset
Textures operator+(const Textures index, int offset);
Textures operator-(const Textures index, int offset);

// File names of the corresponding textures
extern const char* texturesFilesNames[unsigned(Textures::Count)];

#endif  // (PRELOAD_TEXTURES)
