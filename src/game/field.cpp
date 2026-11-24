/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "field.hpp"



Field::Field()
: mines(new Mine[height*width]) {
    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x){
            mines[y * width + x].init(x, y);
        }
    
}

void Field::generateField(const int number, const int WIDTH, const int HEIGHT, coord firstX, coord firstY){
    Uint16* minesToChoose = (Uint16*)malloc(sizeof(Uint16) * WIDTH * HEIGHT);
    count k = 0;
    for(int x = 0; x < WIDTH; ++x)
        for(int y=0; y < HEIGHT; ++y){
            if( (abs(firstX - x) > 1) || (abs(firstY - y) > 1) ){
                minesToChoose[k] = ( x + y * WIDTH ); 
                k++;
            };
    }
    reset();
    
    for(int n=0; n < number; ++n){
        Uint16 chose = rand() % (k-n);
        coord x = minesToChoose[ chose ] % WIDTH;
        coord y = minesToChoose[ chose ] / WIDTH;
        for(int i = 0; i < 8; ++i){
            if( (x + aroundPos[i * 2] >= 0) && 
                (x + aroundPos[i * 2] < WIDTH) && 
                (y + aroundPos[i * 2 + 1] >= 0) && 
                (y + aroundPos[i * 2 + 1] < HEIGHT)){
                int p = (aroundPos[i * 2] + x) + (aroundPos[i * 2 + 1] + y) * WIDTH;
                if(mines[p].state != MINE_MINE){
                    mines[p].state += 1;
                }
            }
        }
        mines[minesToChoose[ chose ]].state = MINE_MINE;
        minesToChoose[ chose ] = minesToChoose[ k - n ];
    };
    free(minesToChoose);

    // Resetting values
    flagSetting = FLAG_NONE;
};

void Field::blit(){
    for(int i=0; i < width * height; ++i){
        mines[i].blit();
    }
};

void Field::LClick(const coord X, const coord Y){
    mines[Y * width + X].Lclick(X, Y);
}

void Field::RClick(const coord X, const coord Y){
    switch (flagSetting)
    {
    case FLAG_NONE:
        if(mines[Y * width + X].state <= MINE_MINE){
            flagSetting = FLAG_SET;
            mines[Y * width + X].FlagSet();
        }
        else{
            flagSetting = FLAG_RESET;
            mines[Y * width + X].FlagReset();
        }
    case FLAG_SET:
        mines[Y * width + X].FlagSet();
        break;
    
    case FLAG_RESET:
        mines[Y * width + X].FlagReset();
        break;
    }
}

void Field::actAround(const coord X, const coord Y){
    mines[X + Y * width].actAround(X, Y);
}

void Field::RUnpress(){
    flagSetting = FLAG_NONE;
}

int Field::getState(const coord x, const coord y){
    return mines[y * width + x].state;
}

void Field::opeanAll(){
    for(int i = 0; i < width * height; ++i){
        mines[i].open();
    }
}

void Field::reset(){
    for(int i = 0; i < width * height; ++i){
        mines[i].state = MINE_NORMAL_0;
        mines[i].texture = Textures[IMG_MINE_CLOSE];
    }
}
