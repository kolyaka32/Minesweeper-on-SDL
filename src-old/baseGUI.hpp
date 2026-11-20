#pragma once

#include "include.hpp"
#include "define.hpp"
#include "structs.hpp"

namespace GUI{

    // Text alignment type
    enum ALIGNMENT_types{
        LEFT_text,
        MIDLE_text,
        RIGHT_text
    };

    // Static text on screen with drawing functions
    class staticText
    {
    private:
        Uint8 fontHeight;
        TTF_Font* Font;
        char *text;
        float posX, posY;
        ALIGNMENT_types aligment;
        SDL_Color color;

        SDL_Texture *Texture;
        SDL_Rect Rect;
    public:
        staticText(char* newText, Uint8 newSize, float newX, float newY, ALIGNMENT_types newAlignment = MIDLE_text, SDL_Color newColor = WHITE);
        ~staticText();
        void init();
        void updateText(LNG_types language, int number = 0);
        void blit();
    };

    // Class of slider bar with point on it better expirience
    class Slider{
    private:
        SDL_Texture *textureLine;
        SDL_Texture *textureButton;
        SDL_Rect destLine;
        SDL_Rect destButton;
    public:
        Slider(const float Y, const IMG_names lineImage, const IMG_names buttonImage);
        void blit(Uint8 state);
        bool in(int X, int Y);
        int getX();
    };

    class Button
    {
    private:
        SDL_Texture *texture;
        SDL_Rect dest;
    public:
        Button(float X, float Y, IMG_names textureIndex);
        void blit();
        bool in(int mouseX, int mouseY);
    };

    #if ANI_count
    class Animation
    {
    private:
        ANI_names type;
        SDL_Texture* texture;
        Uint32 frame;
        Uint64 prevTick;
        SDL_Rect dest;
    public:
        Animation( SDL_Rect destination, ANI_names newType );
        void blit();
        void clear();
    };
    #endif

    class Bar
    {
    private:
        SDL_Rect Back_rect;
        SDL_Rect Front_rect;
        SDL_Rect IconeRect;
        SDL_Texture* IconeTexture;
        SDL_Color color;
    public:
        Bar( const SDL_Rect dest, SDL_Color newColor, IMG_names icone);
        void blit( int width );
    };


/*
    // Class of 
    class DropBox
    {
    private:
        SDL_Rect closeDest;
        SDL_Rect openDest;
        TTF_Font* Font;
        SDL_Texture **Texture;
        SDL_Color backColor;
        SDL_Color frontColor;
        Uint8 count;
        Uint8 state;
        bool open;
        char* text;
    public:
        DropBox(SDL_Rect place, Uint8 newCount, char* newText, Uint8 size, SDL_Color newFrontColor = BLACK, SDL_Color newBackColor = WHITE);
        ~DropBox();
        void updateText(LNG_types language);
        void click(const int mouseX, const int mouseY);
        void blit();
    };*/
}