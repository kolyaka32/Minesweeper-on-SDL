/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "gameCycle.hpp"


GameCycle::GameCycle(Window& _window)
: BaseCycle(_window),
board(_window, 0.26, 0.05, 0.25),
fpsCounter(_window, 0.01, 0.85) {
    if (!isRestarted()) {
        // Resetting field
        board.reset();
        fpsCounter.reset();
    }
    logger.additional("Start game cycle");
}

bool GameCycle::inputMouseDown() {
    if (BaseCycle::inputMouseDown()) {
        return true;
    }
    board.click(mouse);
    return false;
}

void GameCycle::inputMouseUp() {
    mouse.updatePos();
    board.unclick(mouse);
    settings.unClick();
}

bool GameCycle::inputMouseWheel(float _wheelY) {
    if (BaseCycle::inputMouseWheel(_wheelY)) {
        return true;
    }
    if (board.scroll(mouse, _wheelY)) {
        return true;
    }
    return false;
}

bool GameCycle::inputKeys(SDL_Keycode _key) {
    if (BaseCycle::inputKeys(_key)) {
        return true;
    }
    if (_key == SDLK_ESCAPE) {
        // Closing top open object
        settings.activate();
        return true;
    }
    return false;
}

void GameCycle::preUpdate() {
    fpsCounter.start();
}

void GameCycle::update() {
    fpsCounter.setPoint1();

    BaseCycle::update();

    mouse.updatePos();

    board.update(mouse);

    fpsCounter.setPoint2();
}

void GameCycle::postUpdate() {
    fpsCounter.setPoint3();
}

void GameCycle::draw() const {
    // Bliting background
    window.setDrawColor(GREY);
    window.clear();

    // Blitting field
    board.blit();
    fpsCounter.blit();

    // Drawing upper dashboard
    exitButton.blit();
    settings.blit();

    // Bliting all to screen
    window.render();
}
