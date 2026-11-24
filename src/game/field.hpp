/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include <memory>
#include "mine.hpp"


// Whole drawing field with cells on it
class Field {
private:
    static unsigned width, height;

    std::unique_ptr<Mine*> mines;
    Uint8 flagSetting;

public:
    Field();

    // Generating new field of mines with random positions
    void generateField(const int number, const int WIDTH, const int HEIGHT, coord firstX, coord firstY);  
    void reset();  // Resetting states and textures of all mines to basic
    void opeanAll();  // Openning all cell to show their states

    void LClick(const coord x, const coord y);     // Activate current mine and near none-zero mines
    void RClick(const coord x, const coord y);     // Set/unset flag on field
    void actAround(const coord x, const coord y);  // Recurcive activating near mines, if on cell equal to flag number
    void RUnpress();                               // Deactivating right clicking
    int getState(const coord x, const coord y);    // Getting state of need cell
    void blit();                                   // Drawing all mines

    void setWidth(unsigned val);
    void setHeight(unsigned val);
};
