#pragma once

#include "include.h"
#include "define.h"
#include "structs.h"


// Text alignment type
enum ALIGNMENT_types{
    LEFT_text,
    MIDLE_text,
    RIGHT_text
};

// Class of static text on screen
typedef struct{
    const char *text;
    const Uint8 fontSize;
    const float posX, posY;
    
    const enum ALIGNMENT_types aligment;
    const SDL_Color color;

    // Private data
    SDL_Rect Rect;
    TTF_Font* Font;
    SDL_Texture *Texture;
}staticText;

// Functions for working with static text
void initStaticTexts();
void updateStaticText(staticText* obj, int number);
void blitStaticText(staticText* obj);
void freeStaticTexts();


// Class of slider bar with point on it better user expirience
typedef struct{
    const enum IMG_names textureLine;
    const enum IMG_names textureButton;
    const float posY;

    // Private data
    SDL_Rect destLine;
    SDL_Rect destButton;
} Slider;

// Function for working with sliders
void initSlider(Slider* obj);
bool checkInSlider(Slider* obj, int mouseX, int mouseY);
void blitSlider(Slider* obj, Uint8 state);


// Class of slider for volumes setting
typedef struct{
    //
    const float posX, posY;
    const enum IMG_names textureIndex;

    // Private data
    SDL_Rect dest;
} Button;

// Function for working with slider
void initButton(Button* obj);                             // Function of setting generation place for button
bool checkInButton(Button* obj, int mouseX, int mouseY);  // Function, checking, if mouse click on selected button
void blitButton(Button* obj);                             // Function of drawing selected button on screen
