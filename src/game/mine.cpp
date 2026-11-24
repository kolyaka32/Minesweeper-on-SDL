/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "mine.hpp"


void Mine::Lclick(coord X, coord Y){
    if(state == MINE_MINE){  // Exploding mine
        texture = Textures[IMG_MINE_RED];
        state = MINE_EXPLODED;
        loosing = true;
        Mix_PlayChannel(-1, Sounds[SND_LOOSE], 0);
        //gameOver = true;
    }
    else if(state <= MINE_NORMAL_8){  // Open curent mine
        texture = Textures[IMG_MINE_0 + state];
        // Open around cells, if it secure
        state += 10;  // Setting state of cell to open 
        if(state == 10){
            for(int i=0; i < 8; ++i){
                if( ((aroundPos[i*2] + X) >= 0) && 
                    ((aroundPos[i*2] + X) < mineX) && 
                    ((aroundPos[i*2+1] + Y) >= 0) && 
                    ((aroundPos[i*2+1] + Y) < mineY) ){
                        field->LClick(aroundPos[i*2] + X, aroundPos[i*2+1] + Y);
                }
            }
        }
    }
}

void Mine::FlagSet(){
    if(state <= MINE_MINE){
        state += MINE_FLAG_WRONG_0 - MINE_NORMAL_0;
        texture = Textures[IMG_FLAG_RIGHT];
        flagedMines++;
        if(state == MINE_FLAG_CORRECT){
            realOpenMines++;
        }
    }
}

void Mine::FlagReset(){
    if(state >= MINE_FLAG_WRONG_0){
        state -= MINE_FLAG_WRONG_0 - MINE_NORMAL_0;
        texture = Textures[IMG_MINE_CLOSE];
        flagedMines--;
        if(state == MINE_MINE){
            realOpenMines--;
        }
    }
}

void Mine::actAround(coord X, coord Y){
    if(state > 9 && state < 19){
        int around = 0;
        for(int i=0; i < 8; ++i){
            if( ((aroundPos[i*2] + X) >= 0) && 
                ((aroundPos[i*2] + X) < mineX) && 
                ((aroundPos[i*2+1] + Y) >= 0) && 
                ((aroundPos[i*2+1] + Y) < mineY) &&
                (field->getState(aroundPos[i*2] + X, aroundPos[i*2+1] + Y) >= MINE_FLAG_WRONG_0))
                    around += 1;
                
        }
        if(state == around + 10 && state != 10){
            for(int i=0; i<8; ++i){
                if( ((aroundPos[i*2] + X) >= 0) && 
                    ((aroundPos[i*2] + X) < mineX) && 
                    ((aroundPos[i*2+1] + Y) >= 0) && 
                    ((aroundPos[i*2+1] + Y) < mineY) )
                        field->LClick(aroundPos[i*2] + X, aroundPos[i*2+1] + Y);
            }
        }
    }
}

void Mine::open(){
    if(state == MINE_MINE){
        texture = Textures[IMG_MINE_NORMAL];
    }
    else if(state == MINE_FLAG_CORRECT){
        texture = Textures[IMG_MINE_GREEN];
    }
    else if(state >= MINE_FLAG_WRONG_0){
        texture = Textures[IMG_FLAG_WRONG];
    }
}

void Mine::blit(const Window& _window, SDL_FRect _rect) {

    SDL_RenderCopy(app.renderer, texture, NULL, &dest); 
}
