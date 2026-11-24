/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "musicNames.hpp"

#if (PRELOAD_MUSIC)

// File names of the corresponding music
const char* musicFilesNames[unsigned(Music::Count)] = {
    "mus/main_theme.mp3",
    "mus/menu_theme.mp3",
};

#endif  // (PRELOAD_MUSIC)
