/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "../data/app.hpp"


// Type for coordinates
typedef uint8_t coord;

// Mine class
class Mine {
private:
    // Types of mines
    enum {
        // Cell base part (first 4 bits)
        Cell0 = 0,
        Cell8 = 8,
        CellMine = 15,

        // Cell with flag on it (5 bit)
        CellFlag = 16,

        // Cell showing state (6 bit)
        CellOpen = 32,

        MineExploded = CellMine | CellOpen,
        FlagCorrect  = CellMine | CellFlag,
    };
    // State of mine (OR'd together)
    Sint16 state;

public:
    void init(coord X, coord Y);       // Function of setting mine at his need position
    void Lclick(coord X, coord Y);     // Function of activation cell
    void FlagSet();                    // Function of flagging cell
    void FlagReset();                  // Function of unflagging cell
    void actAround(coord X, coord Y);  // Function of testing around cell to try activate them
    void open();                       // Final function of showing internal state of cell

    void blit(const Window& window, SDL_FRect rect);
};
