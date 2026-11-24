/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "texturesNames.hpp"

#if (PRELOAD_TEXTURES)


// File names of the corresponding textures
const char* texturesFilesNames[unsigned(Textures::Count)] = {
    // Graphic interface sprites
    "img/slider_button.png",
    "img/slider_line.png",
    "img/esc_button.png",

    "img/Flag_USA.png",
    "img/Flag_RUS.png",

    "img/but.png",
    "img/but10.png",
    "img/but12.png",
    "img/but11.png",

    "img/but0.png",
    "img/but1.png",

    "img/num_0.png",
    "img/num_1.png",
    "img/num_2.png",
    "img/num_3.png",
    "img/num_4.png",
    "img/num_5.png",
    "img/num_6.png",
    "img/num_7.png",
    "img/num_8.png",
};

Textures operator+(const Textures _color, int _offset) {
    return Textures(unsigned(_color) + _offset);
}

Textures operator-(const Textures _color, int _offset) {
    return Textures(unsigned(_color) - _offset);
}

#endif  // (PRELOAD_TEXTURES)
