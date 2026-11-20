/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "data/app.hpp"
#include "cycles/gameCycle.hpp"


// Starting cycle
Cycle App::nextCycle = Cycle::Main;

void App::run(Window& _window) {
    logger.additional("\nStart app");

    // Starting loop of selecting cycles
    while (running) {
        // Selecting new
        switch (nextCycle) {
        case Cycle::Main:
            runCycle<GameCycle>(_window);
            break;
        
        default:
            running = false;
            break;
        }
    }
}
