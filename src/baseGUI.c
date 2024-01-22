#include "include.h"
#include "define.h"
#include "structs.h"
#include "dataLoader.h"
#include "baseGUI.h"
#include "pause.h"

#define BUFFER_LENGTH 100

// Function of creating font with need height
TTF_Font* createFont(Uint8 size){
    SDL_RWops* fontData = SDL_RWFromMem(fontMemory, fontSize);
    return TTF_OpenFontRW(fontData, 1, size);
};


// Functions for working with static text
void initStaticTexts(){
    for(Uint8 i=0; i<TXT_count; ++i){
        texts[i].Font = createFont(texts[i].fontSize);
        texts[i].Rect = (SDL_Rect){0, texts[i].posY * SCREEN_HEIGHT, 0, 0};
    }
}


void updateStaticText(staticText* obj, int number){
    char buffer[BUFFER_LENGTH];
    Uint8 start = 0;
    for(Uint8 end = 0; (end != language) && (start < BUFFER_LENGTH); ++start){
        if(obj->text[start] == '\n'){
            end++;
        }
    }
    Uint8 d = 0;
    for(int i = start; (obj->text[i] != '\n') && (start < BUFFER_LENGTH); ++i ){
        switch (obj->text[i])
        {
        case '%':
            if(number < 0){
                buffer[d++] = '-';
                number = -number;
            }
            Uint8 end = 0;
            int num = number;
            do{
                num/=10;
                end++;
            } while(num);
            d += end;
            do {   
                buffer[--d] = '0' + number % 10;
                number /= 10;
            } while (number);
            d += end;
            break;
        
        default:
            buffer[d++] = obj->text[i];
            break;
        }
    }
    buffer[d] = '\0';
    
    SDL_Surface* Surface = TTF_RenderUTF8_Solid(obj->Font, buffer, obj->color);
    obj->Texture = SDL_CreateTextureFromSurface(app.renderer, Surface);
    SDL_FreeSurface(Surface);
    SDL_QueryTexture(obj->Texture, NULL, NULL, &obj->Rect.w, &obj->Rect.h);
    obj->Rect.x = (SCREEN_WIDTH) * obj->posX - (obj->Rect.w * obj->aligment / 2);
};

//
void blitStaticText(staticText* obj){
    SDL_RenderCopy(app.renderer, obj->Texture, NULL, &obj->Rect);
};

void freeStaticTexts(){
    for(Uint8 i=0; i < TXT_count; ++i){
        SDL_DestroyTexture(texts[i].Texture);
        TTF_CloseFont(texts[i].Font);
    }
};

// Function for working with sliders
void initSlider(Slider* obj){
    SDL_QueryTexture(Textures[obj->textureLine], NULL, NULL, &obj->destLine.w, &obj->destLine.h);
    SDL_QueryTexture(Textures[obj->textureButton], NULL, NULL, &obj->destButton.w, &obj->destButton.h);
    obj->destLine.x = (SCREEN_WIDTH - obj->destLine.w)/2;
    obj->destLine.y = SCREEN_HEIGHT * obj->posY - obj->destLine.h / 2;
    obj->destButton.y = SCREEN_HEIGHT * obj->posY - obj->destButton.h / 2;
};

bool checkInSlider(Slider* obj, int mouseX, int mouseY){
    return ((mouseX > obj->destLine.x && mouseX < obj->destLine.x + obj->destLine.w) &&
        (mouseY > obj->destLine.y && mouseY < obj->destLine.y + obj->destLine.h));
};

void blitSlider(Slider* obj, Uint8 state){
    obj->destButton.x = obj->destLine.x + state - obj->destButton.w/2;
    SDL_RenderCopy(app.renderer, Textures[obj->textureLine], NULL, &obj->destLine);
    SDL_RenderCopy(app.renderer, Textures[obj->textureButton], NULL, &obj->destButton);
};

// Function for working with slider
// Function of setting generation place for button
void initButton(Button* obj){
    SDL_QueryTexture(Textures[obj->textureIndex], NULL, NULL, &obj->dest.w, &obj->dest.h);
    obj->dest.x = SCREEN_WIDTH * obj->posX - obj->dest.w/2;
    obj->dest.y = SCREEN_HEIGHT * obj->posY - obj->dest.h/2;
};

// Function, checking, if mouse click on selected button
bool checkInButton(Button* obj, int mouseX, int mouseY){
    return ((mouseX > obj->dest.x && mouseX < obj->dest.x + obj->dest.w) &&
        (mouseY > obj->dest.y && mouseY < obj->dest.y + obj->dest.h));
};

void blitButton(Button* obj){
    SDL_RenderCopy(app.renderer, Textures[obj->textureIndex], NULL, &obj->dest);
};
