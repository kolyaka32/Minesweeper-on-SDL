#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "structs.h"

#include "initFile.h"
#include "pause.h"

//
#define BUFFER_LENGTH 100

// Loading initialasing settings in game
void loadInitFile(){
    // Creating buffer for reading line
    char buffer[BUFFER_LENGTH];
    // Open file to read
    FILE* inSettings = fopen(SETTING_FILE, "r");

    // Setting standart values for variables
    language = LNG_ENGLISH;
    MusicVolume = MIX_MAX_VOLUME/2;
    EffectsVolume = MIX_MAX_VOLUME/2;
    drawFPS = 60;
    mineX = 16;
    mineY = 20;
    mineCount = 40;
    leftTimer = 0;

    Uint8 d = 0;
    int* data = 0;
    Uint8 dataSize = 0;
    for(char l; l != -1;){
        l = fgetc(inSettings);
        switch (l)
        {
        case ' ':
            break;

        case '\n':
            buffer[d] = '\0';
            if(data == &language){
                if(strcmp(buffer, "english") == 0){
                    language = LNG_ENGLISH;
                }
                else if(strcmp(buffer, "russian") == 0){
                    language = LNG_RUSSIAN;
                }
            }
            else if(data != 0){
                Uint16 readData = 0;
                for(Uint8 i = 0; i < d; ++i){
                    if(buffer[i] >= '0' && buffer[i] <= '9')
                        readData = readData * 10 + buffer[i] - '0';
                }
                memcpy(data, &readData, dataSize);
            }
            data = 0;
            dataSize = 0;;
            d = 0;
            break;

        case '=':
            buffer[d] = '\0';

            if(strcmp(buffer, "language") == 0){
                data = &language;
                dataSize = sizeof(language);
            }
            else if(strcmp(buffer, "music") == 0){
                data = &MusicVolume;
                dataSize = sizeof(MusicVolume);
            }
            else if(strcmp(buffer, "effects") == 0){
                data = &EffectsVolume;
                dataSize = sizeof(EffectsVolume);
            }
            else if(strcmp(buffer, "maxfps") == 0){
                data = &drawFPS;
                dataSize = sizeof(drawFPS);
            }
            else if(strcmp(buffer, "width") == 0){
                data = &mineX;
                dataSize = sizeof(mineX);
            }
            else if(strcmp(buffer, "height") == 0){
                data = &mineY;
                dataSize = sizeof(mineY);
            }
            else if(strcmp(buffer, "count") == 0){
                data = &mineCount;
                dataSize = sizeof(mineCount);
            }
            else if(strcmp(buffer, "playtime") == 0){
                data = &leftTimer;
                dataSize = sizeof(leftTimer);
            }
            else
                data = NULL;

            d = 0;
            break;
        
        default:
            if(d < BUFFER_LENGTH)
                buffer[d++] = SDL_tolower(l);
            break;
        }
    }
    // Checking grid for getting over screen
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);

    if(mineX < 5){
        mineX = 5;
    }
    if(mineX > mode.w / MINE_SIDE){
        mineX = mode.w / MINE_SIDE;
    }
    if(mineY < 5){
        mineY = 5;
    }
    if(mineY > (mode.h - HEADLINE - 200) / MINE_SIDE){
        mineY = (mode.h - HEADLINE - 200) / MINE_SIDE;
    }
    // Checking grid for getting over screen
    if(mineCount > mineX * mineY - 9){
        mineCount = mineX * mineY - 9;
    }
    if(mineCount < 1){
        mineCount = 1;
    }
    // Checking for FPS counter
    if(drawFPS < 2){
        drawFPS = 2;
    }

    // Closing reading file
    fclose(inSettings);
}

// Saving initialasing file
void saveInitFile(){
    // Creating output file
    FILE* outSettings = fopen(SETTING_FILE, "w");

    // Writing data to output
    fprintf(outSettings, "# Language type (english/russian):\n");// Extra comment
    switch (language)  // Writing language
    {
    case LNG_RUSSIAN:
        fprintf(outSettings, "language = russian\n");
        break;
    case LNG_ENGLISH:
    default:
        fprintf(outSettings, "language = english\n");
        break;
    }
    fprintf(outSettings, "\n# Technical part:\n");               // Extra comment
    fprintf(outSettings, "music = %u\n", MusicVolume);         // Writing music volume
    fprintf(outSettings, "effects = %u\n", EffectsVolume);     // Writing effects volume
    fprintf(outSettings, "max FPS = %u\n", drawFPS);           // Writing frames per seconds

    fprintf(outSettings, "\n# Sizes of field:\n");               // Extra comment
    fprintf(outSettings, "width = %d\n", mineX);               // Writing width of grid in mines
    fprintf(outSettings, "height = %d\n", mineY);              // Writing height of grid in mines

    fprintf(outSettings, "\n# Count of mines on game field\n");  // Extra comment
    fprintf(outSettings, "count = %u\n", mineCount);           // Writing number of mines

    fprintf(outSettings, "\n# Time to solve puzzle, leave 0 if not enable\n");
    fprintf(outSettings, "playtime = %d\n", leftTimer);       // Writing time for fully solving

    fclose(outSettings);
}