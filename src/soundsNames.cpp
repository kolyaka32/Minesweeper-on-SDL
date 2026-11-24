/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "soundsNames.hpp"

#if (PRELOAD_SOUNDS)


// File names of the corresponding sounds
const char* soundsFilesNames[unsigned(Sounds::Count)] = {
    "snd/click.wav",
    "snd/flagSet.wav",
    "snd/loose.wav",
    "snd/win.wav",
};

#endif  // (PRELOAD_SOUNDS)
