#include "include.hpp"
#include "structs.hpp"
#include "dataLoader.hpp"

void Mine::init(coord X, coord Y){
    dest.x = X * MINE_SIDE;
    dest.y = Y * MINE_SIDE + HEADLINE;
    dest.w = dest.h = MINE_SIDE;
    state = MINE_NORMAL_0;
    texture = Textures[IMG_MINE_CLOSE];
}

void Mine::blit(){
    SDL_RenderCopy(app.renderer, texture, NULL, &dest); 
}

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


// Field class
Field::Field(){
    mines = (Mine*)malloc(sizeof(Mine) * mineX * mineY);
    for(int y = 0; y < mineY; ++y)
        for(int x = 0; x < mineX; ++x){
            mines[y * mineX + x].init(x, y);
        }
    
}

Field::~Field(){
    free(mines);
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
    for(int i=0; i < mineX * mineY; ++i){
        mines[i].blit();
    }
};

void Field::LClick(const coord X, const coord Y){
    mines[Y * mineX + X].Lclick(X, Y);
}

void Field::RClick(const coord X, const coord Y){
    switch (flagSetting)
    {
    case FLAG_NONE:
        if(mines[Y * mineX + X].state <= MINE_MINE){
            flagSetting = FLAG_SET;
            mines[Y * mineX + X].FlagSet();
        }
        else{
            flagSetting = FLAG_RESET;
            mines[Y * mineX + X].FlagReset();
        }
    case FLAG_SET:
        mines[Y * mineX + X].FlagSet();
        break;
    
    case FLAG_RESET:
        mines[Y * mineX + X].FlagReset();
        break;
    }
}

void Field::actAround(const coord X, const coord Y){
    mines[X + Y * mineX].actAround(X, Y);
}

void Field::RUnpress(){
    flagSetting = FLAG_NONE;
}

int Field::getState(const coord x, const coord y){
    return mines[y * mineX + x].state;
}

void Field::opeanAll(){
    for(int i = 0; i < mineX * mineY; ++i){
        mines[i].open();
    }
}

void Field::reset(){
    for(int i = 0; i < mineX * mineY; ++i){
        mines[i].state = MINE_NORMAL_0;
        mines[i].texture = Textures[IMG_MINE_CLOSE];
    }
}