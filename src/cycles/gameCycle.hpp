/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "baseCycle.hpp"
#include "../game/boardInteracter.hpp"
#include "../game/fpsCounter.hpp"


// Cycle with game
class GameCycle : public BaseCycle {
 protected:
    // Active game part
    BoardInteracter board;
    FPScounter fpsCounter;

    // New overrided cycle functions
    bool inputMouseDown() override;
    void inputMouseUp() override;
    bool inputMouseWheel(float wheelY) override;
    bool inputKeys(SDL_Keycode key) override;
    void update() override;
    void preUpdate() override;
    void postUpdate() override;
    void draw() const override;

 public:
    GameCycle(Window& _window);
};
