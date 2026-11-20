/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "texturesNames.hpp"

#if (PRELOAD_TEXTURES)


// File names of the corresponding textures
const char* texturesFilesNames[unsigned(Textures::Count)] = {
    // Graphic interface sprites
    "img/GUI/slider_button.png",
    "img/GUI/slider_line.png",
    "img/GUI/quit_button.png",
    "img/GUI/menu_button.png",
    "img/GUI/save_button.png",
    //"img/GUI/restart_button.png",
    "img/GUI/esc_button.png",

    // Flags
    "img/GUI/Flag_USA.png",
    "img/GUI/Flag_RUS.png",
    //"img/GUI/Flag_GER.png",
    //"img/GUI/Flag_BEL.png",

    // Game part
    "img/game/wall.png",
    "img/game/vent.png",
    "img/game/heater.png",
    "img/game/buldozer.png",
    "img/game/checkValve.png",
    "img/game/cooler.png",
};

Textures operator+(const Textures _color, int _offset) {
    return Textures(unsigned(_color) + _offset);
}

Textures operator-(const Textures _color, int _offset) {
    return Textures(unsigned(_color) - _offset);
}

#endif  // (PRELOAD_TEXTURES)
