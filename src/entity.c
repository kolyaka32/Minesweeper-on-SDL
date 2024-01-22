#include "include.h"
#include "structs.h"
#include "dataLoader.h"

// Array of positions, near random cell
static const Sint8 aroundPos[16] = {
    0, 1,
    1, 1,
    1, 0,
    1, -1,
    0, -1,
    -1, -1,
    -1, 0,
    -1, 1
};

// Function of setting mine at his need position
void mineInit(Mine* mine, coord X, coord Y){
    mine->dest.x = X * MINE_SIDE;
    mine->dest.y = Y * MINE_SIDE + HEADLINE;
    mine->dest.w = mine->dest.h = MINE_SIDE;
    mine->state = MINE_NORMAL_0;
    mine->texture = Textures[IMG_MINE_CLOSE];
}

void mineLMBclick(Mine* mine, coord X, coord Y){
    if(mine->state == MINE_MINE){  // Exploding mine
        mine->texture = Textures[IMG_MINE_RED];
        mine->state = MINE_EXPLODED;
        loosing = true;
        Mix_PlayChannel(-1, Sounds[SND_LOOSE], 0);
    }
    else if(mine->state <= MINE_NORMAL_8){  // Open curent mine
        mine->texture = Textures[IMG_MINE_0 + mine->state];
        // Open around cells, if it secure
        mine->state += 10;  // Setting state of cell to open 
        if(mine->state == 10){
            for(int i=0; i < 8; ++i)
                if( ((aroundPos[i*2] + X) >= 0) && 
                    ((aroundPos[i*2] + X) < mineX) && 
                    ((aroundPos[i*2+1] + Y) >= 0) && 
                    ((aroundPos[i*2+1] + Y) < mineY) )
                        fieldLMBClick(aroundPos[i*2] + X, aroundPos[i*2+1] + Y);
        }
    }
}

void mineFlagSet(Mine* mine){
    if(mine->state <= MINE_MINE){
        mine->state += MINE_FLAG_WRONG_0 - MINE_NORMAL_0;
        mine->texture = Textures[IMG_FLAG_RIGHT];
        flagedMines++;
        if(mine->state == MINE_FLAG_CORRECT){
            realOpenMines++;
        }
    }
}

void mineFlagReset(Mine* mine){
    if(mine->state >= MINE_FLAG_WRONG_0){
        mine->state -= MINE_FLAG_WRONG_0 - MINE_NORMAL_0;
        mine->texture = Textures[IMG_MINE_CLOSE];
        flagedMines--;
        if(mine->state == MINE_MINE){
            realOpenMines--;
        }
    }
}

void mineActAround(Mine* mine, coord X, coord Y){
    if(mine->state > 9 && mine->state < 19){
        Uint8 around = 0;
        for(int i=0; i < 8; ++i){
            if( ((aroundPos[i*2] + X) >= 0) && 
                ((aroundPos[i*2] + X) < mineX) && 
                ((aroundPos[i*2+1] + Y) >= 0) && 
                ((aroundPos[i*2+1] + Y) < mineY) &&
                (fieldGetState(aroundPos[i*2] + X, aroundPos[i*2+1] + Y) >= MINE_FLAG_WRONG_0))
                    around += 1;
        }
        if(mine->state != MINE_FLAG_WRONG_0 && mine->state == around + 10){
            for(int i=0; i<8; ++i){
                if( ((aroundPos[i*2] + X) >= 0) && 
                    ((aroundPos[i*2] + X) < mineX) && 
                    ((aroundPos[i*2+1] + Y) >= 0) && 
                    ((aroundPos[i*2+1] + Y) < mineY) )
                        fieldLMBClick(aroundPos[i*2] + X, aroundPos[i*2+1] + Y);
            }
        }
    }
}

void newField(){
    // Setting memory for struct field
    field = (Field*)malloc(sizeof(Field));
    field->mines = (Mine*)malloc(sizeof(Mine) * mineX * mineY);
    for(int y = 0; y < mineY; ++y)
        for(int x = 0; x < mineX; ++x){
            mineInit(field->mines + y * mineX + x, x, y);
        }
}

void freeField(Field* f){
    free(f->mines);
    free(f);
}


void fieldGenerate(const int number, const int WIDTH, const int HEIGHT, coord firstX, coord firstY){
    Uint16* minesToChoose = (Uint16*)malloc(sizeof(Uint16) * WIDTH * HEIGHT);
    count k = 0;
    for(int x = 0; x < WIDTH; ++x)
        for(int y=0; y < HEIGHT; ++y){
            if( (abs(firstX - x) > 1) || (abs(firstY - y) > 1) ){
                minesToChoose[k] = ( x + y * WIDTH ); 
                k++;
            };
    }

    for(int i = 0; i < mineX * mineY; ++i){
        field->mines[i].state = MINE_NORMAL_0;
        field->mines[i].texture = Textures[IMG_MINE_CLOSE];
    }
    
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
                if(field->mines[p].state != MINE_MINE){
                    field->mines[p].state += 1;
                }
            }
        }
        field->mines[minesToChoose[ chose ]].state = MINE_MINE;
        minesToChoose[ chose ] = minesToChoose[ k - n ];
    };
    free(minesToChoose);

    // Resetting values
    field->flagSetting = FLAG_NONE;
};

void fieldBlit(){
    for(int i=0; i < mineX * mineY; ++i){
        SDL_RenderCopy(app.renderer, field->mines[i].texture, NULL, &field->mines[i].dest); 
    }
};

void fieldLMBClick(const coord X, const coord Y){
    mineLMBclick(field->mines + Y * mineX + X, X, Y);
}

void fieldRMBClick(const coord X, const coord Y){
    switch (field->flagSetting)
    {
    case FLAG_NONE:
        if(field->mines[Y * mineX + X].state <= MINE_MINE){
            field->flagSetting = FLAG_SET;
            mineFlagSet(field->mines + Y * mineX + X);
        }
        else{
            field->flagSetting = FLAG_RESET;
            mineFlagReset(field->mines + Y * mineX + X);
        }
    case FLAG_SET:
        mineFlagSet(field->mines + Y * mineX + X);
        break;
    
    case FLAG_RESET:
        mineFlagReset(field->mines + Y * mineX + X);
        break;
    }
}

void fieldRMBUnpress(){
    field->flagSetting = FLAG_NONE;
}

void fieldActAround(const coord X, const coord Y){
    mineActAround(field->mines + X + Y * mineX, X, Y);
}

int fieldGetState(const coord X, const coord Y){
    return field->mines[Y * mineX + X].state;
}

void fieldOpeanAll(){
    for(int i = 0; i < mineX * mineY; ++i){
        Mine* mine_t = field->mines + i;
        if(mine_t->state == MINE_MINE){
            mine_t->texture = Textures[IMG_MINE_NORMAL];
        }
        else if(mine_t->state == MINE_FLAG_CORRECT){
            mine_t->texture = Textures[IMG_MINE_GREEN];
        }
        else if(mine_t->state >= MINE_FLAG_WRONG_0){
            mine_t->texture = Textures[IMG_FLAG_WRONG];
        }
    }
}

void fieldReset(){
    for(int i = 0; i < mineX * mineY; ++i){
        field->mines[i].state = MINE_NORMAL_0;
        field->mines[i].texture = Textures[IMG_MINE_CLOSE];
    }
}