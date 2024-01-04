#include "include.hpp"
#include "define.hpp"
#include "pause.hpp"

// Types of selected box
enum{  
    NORMAL_BOX,
    MUSIC_SLIDER_BOX,
    EFFECT_SLIDER_BOX,
    ENGLISH_BOX,
    RUSSIAN_BOX,
    ESCAPE_BOX
} SELCTED_BOX_types;


void setAllText(){
    // Main game texts
    texts[TXT_START_KEYS] = new GUI::staticText(
        "Press on field to start\0"
        "Нажмите на поле для старта\0"
        , 24, 0.5, 0.08);
    
    texts[TXT_MINE_REST] = new GUI::staticText(
        "Rest: %\0"
        "Осталось: %\0"
        , 24, 0.02, 0.02, GUI::LEFT_text);
    texts[TXT_TIME] = new GUI::staticText(
        "Time: %\0"
        "Время: %\0"
        , 24, 0.9, 0.02, GUI::RIGHT_text);
    
    texts[TXT_END_WIN] = new GUI::staticText(
        "You win!\0"
        "Вы выиграли!\0"
        , 24, 0.5, 0.06);
    texts[TXT_END_LOOSE] = new GUI::staticText(
        "You loose...\0"
        "Вы проиграли...\0"
        , 24, 0.5, 0.06);
    texts[TXT_END_SKIP] = new GUI::staticText(
        "You stop game.\0"
        "Вы остановили игру.\0"
        , 24, 0.5, 0.06);
    texts[TXT_END_KEYS] = new GUI::staticText(
        "Press any button.\0"
        "Нажмите на любую кнопку.\0"
        , 24, 0.5, 0.11);
    
    texts[TXT_PAUSE_PAUSE] = new GUI::staticText(
        "Game on pause\0"
        "Игра на паузе\0"
        , 30, 0.5, 0.05, GUI::MIDLE_text, GREY);
    texts[TXT_PAUSE_MUSIC] = new GUI::staticText(
        "Music:\0"
        "Музыка:\0"
        , 24, 0.5, 0.4, GUI::MIDLE_text, GREY);
    texts[TXT_PAUSE_SOUND] = new GUI::staticText(
        "Sounds:\0"
        "Звуки:\0"
        , 24, 0.5, 0.55, GUI::MIDLE_text, GREY);
    
    // Buttons
    esc = new GUI::Button(0.97, 0.03, IMG_MENU_PAUSE);
}

void updateTranslation(LNG_types language){
    // Setting fruit skin
    switch (language)
    {
    case LNG_ENGLISH:
        SDL_SetWindowTitle(app.window, "Minesweeper on SDL");
        break;

    case LNG_RUSSIAN:
        SDL_SetWindowTitle(app.window, "Сапёр на SDL");
        break;
    }
    
    // Updating texts
    for(int i=0; i<TXT_count;++i){
        texts[i]->updateText(language);
    }
    
    // Updating dinamic texts
    texts[TXT_MINE_REST]->updateText(language, mineCount - flagedMines);
    // Updating timer value, depend, if not equal 0 - rest time
    if(leftTimer){
        texts[TXT_TIME]->updateText(language, leftTimer - (SDL_GetTicks64() - screenTimer) / 1000);
    }
    else{
        texts[TXT_TIME]->updateText(language, (SDL_GetTicks64() - screenTimer) / 1000);
    }
}

void clearAllText(){
    for(int i=0; i<TXT_count;++i){
        delete texts[i];
    }
    delete esc;
}

// Pause menu
void pause(){
    // Save time, when pause start
    Uint64 saveTimer = SDL_GetTicks64();

    // Creating pause text
    GUI::Slider MusicSlider(0.46, IMG_MENU_SCROLLER_LINE, IMG_MENU_SCROLLER_BUTTON);
    GUI::Slider SoundSlider(0.61, IMG_MENU_SCROLLER_LINE, IMG_MENU_SCROLLER_BUTTON);
    GUI::Button BtnFlagUSA(0.3, 0.2, IMG_FLAG_USA );
    GUI::Button BtnFlagRUS(0.7, 0.2, IMG_FLAG_RUS );
    /*GUI::DropBox* sizeDropBox = new GUI::DropBox({SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT * 0.8, 80, 24}, 4, 
    "123\n456\n789\n101\n",
    20);*/

    //sizeDropBox->updateText(language);

    SDL_Event event;
    bool waiting = true;
    bool MouseDown = false;
    char inBox = NORMAL_BOX;
    LNG_types newLanguage = language;
    Uint8 backMove = 0;

    #if SCROLLER_SOUND
    Uint64 prevSND = SDL_GetTicks64();
    #endif

    // Starting loop for waiting for start
    while(waiting){  
        while( SDL_PollEvent(&event) != 0 ){
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;  // Exit from program
                waiting = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    waiting = false;  // Returning to game
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                MouseDown = true;
                break;

            case SDL_MOUSEBUTTONUP:
                MouseDown = false; 
                inBox = NORMAL_BOX;
            }
        }

        int MouseX, MouseY;
        SDL_GetMouseState(&MouseX, &MouseY);  // Getting mouse position
        if(MouseDown && inBox == NORMAL_BOX){
            if(MusicSlider.in(MouseX, MouseY)){
                inBox = MUSIC_SLIDER_BOX;
            }
            else if(SoundSlider.in(MouseX, MouseY)){
                inBox = EFFECT_SLIDER_BOX;
            }
            else if(BtnFlagUSA.in(MouseX, MouseY)){
                inBox = ENGLISH_BOX;
            }
            else if(BtnFlagRUS.in(MouseX, MouseY)){
                inBox = RUSSIAN_BOX;
            }
            else if(esc->in(MouseX, MouseY)){
                inBox = ESCAPE_BOX;
            }
            else{
                inBox = NORMAL_BOX;
            }
        }
        //sizeDropBox->click(MouseX, MouseY);

        switch(inBox)
        {
        case MUSIC_SLIDER_BOX:  // If touch music slider
            MusicVolume = (MouseX - MusicSlider.getX()) / 2;
            if(MouseX - MusicSlider.getX() < 0) MusicVolume = 0;
            if(MouseX - MusicSlider.getX() > 255) MusicVolume = 255;
            Mix_VolumeMusic(MusicVolume);  // Setting volume of music
            break;
        case EFFECT_SLIDER_BOX:  // If touch effects slider
            EffectsVolume = (MouseX - SoundSlider.getX()) / 2;
            if(MouseX - SoundSlider.getX() < 0) EffectsVolume = 0;
            if(MouseX - SoundSlider.getX() > 255) EffectsVolume = 255;
            Mix_Volume(-1, EffectsVolume);  // Setting volume of effects
            
            // Playing sound effect for understanding loud
            #if SCROLLER_SOUND
            if( SDL_GetTicks64() - prevSND > 200 ){
                Mix_PlayChannel(-1, Sounds[SND_FLAG_SET], 0);
                prevSND = SDL_GetTicks64();
            }
            #endif
            break;
        case ENGLISH_BOX:  // If touch english language box
            newLanguage = LNG_ENGLISH;
            break;
        case RUSSIAN_BOX:  // If touch russian language box
            newLanguage = LNG_RUSSIAN;
            break;
        case ESCAPE_BOX:
            waiting = false;
            break;
        }

        // Checking for changing language
        if(newLanguage != language){
            language = newLanguage;
            updateTranslation(language);
        }

        // Drawing
        SDL_RenderClear(app.renderer);

        // Filling all screen with closed mines under angle
        for(int y = -(mineY + HEADLINE/MINE_SIDE); y < (mineY + HEADLINE/MINE_SIDE)*0.8; ++y)
            for(int x = -mineX; x < mineX; ++x){
                SDL_Rect dest = {SCREEN_WIDTH/2 + x * MINE_SIDE + backMove/2, SCREEN_HEIGHT/2 + y * MINE_SIDE + backMove, MINE_SIDE, MINE_SIDE};
                SDL_Point point = {-x * MINE_SIDE - backMove/2, -y * MINE_SIDE - backMove};  // Getting to center of screen
                SDL_RenderCopyEx(app.renderer, Textures[IMG_MINE_0], NULL, &dest, 30, &point, SDL_FLIP_NONE);
            }
        backMove++;
        backMove %= (MINE_SIDE * 2);
         
        // Showing extra text
        texts[TXT_PAUSE_PAUSE]->blit();
        texts[TXT_PAUSE_MUSIC]->blit();
        texts[TXT_PAUSE_SOUND]->blit();
        // Drawing sliders
        MusicSlider.blit(MusicVolume * 2);
        SoundSlider.blit(EffectsVolume * 2);
        // Drawing buttons
        esc->blit();
        BtnFlagUSA.blit();
        BtnFlagRUS.blit();
        //sizeDropBox->blit();

        // Blitting textures on screen
        SDL_RenderPresent(app.renderer);  

        // Delaying time to decrease CPU loading
        SDL_Delay(1000 / drawFPS);  
    }
    // Clearing dinamic structs
    //delete sizeDropBox;

    // Resetting timer to save rest time
    screenTimer += (SDL_GetTicks64() - saveTimer);
    //screenTimer = SDL_GetTicks64();
};

void endMenu(){
    // Opening all cells to show correction
    field->opeanAll();

    // Stopping palying music, while waiting for restart
    Mix_PauseMusic();

    // Starting loop for waiting for start
    bool waiting = true;
    SDL_Event event;
    while(waiting && running){
        while( SDL_PollEvent(&event) != 0 ){  // Getting events
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;  // Exit from program
                waiting = false;
                break;
            
            case SDL_KEYDOWN:
                // Going to pause menu by escape button
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    pause();  
                    break;
                
                default:
                    waiting = false;
                    break;
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                int MouseX, MouseY;
                SDL_GetMouseState(&MouseX, &MouseY);  // Getting mouse position
                if(esc->in(MouseX, MouseY)){  // Clicking on escape button
                    pause();
                }
                break;
            }
        }
        // Drawing
        SDL_RenderClear(app.renderer);

        // Drawing, what rest from field
        field->blit();

        // Drawing signs
        texts[TXT_MINE_REST]->blit();
        texts[TXT_TIME]->blit();

        if(loosing){
            texts[TXT_END_LOOSE]->blit();
        }
        else if(winning){
            texts[TXT_END_WIN]->blit();
        }
        else if(skipping){
            texts[TXT_END_SKIP]->blit();
        }
        texts[TXT_END_KEYS]->blit();

        esc->blit();

        SDL_RenderPresent(app.renderer);

        SDL_Delay( 1000/drawFPS );    // Delaying constant time between ticks to decrease CPU loading
    }
    field->reset();

    // Resuming playing menu music
    Mix_PlayMusic( Musics[MUS_MENU_THEME], -1 );
    Mix_ResumeMusic();
}

void startMenu(){
    // Updating score
    texts[TXT_MINE_REST]->updateText(language, 0);
    texts[TXT_TIME]->updateText(language, 0);

    // Starting loop for waiting for start
    bool waiting = true;
    SDL_Event event;
    while(waiting && running){
        while( SDL_PollEvent(&event) != 0 ){  // Getting events
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;  // Exit from program
                waiting = false;
                break;
            
            case SDL_KEYDOWN:
                // Going to pause menu by escape button
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    pause();  
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                int MouseX, MouseY;
                SDL_GetMouseState(&MouseX, &MouseY);  // Getting mouse position
                if(esc->in(MouseX, MouseY)){  // Clicking on escape button
                    pause();
                }
                else if(MouseY > HEADLINE){
                    // Generating new field
                    field->generateField(mineCount, mineX, mineY, MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
                    // First click
                    field->LClick(MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
                    waiting = false;
                }
                break;
            }
        }
        // Drawing
        SDL_RenderClear(app.renderer);

        field->blit();
        // Drawing signes after bacground
        texts[TXT_START_KEYS]->blit();

        texts[TXT_MINE_REST]->blit();
        texts[TXT_TIME]->blit();

        esc->blit();
        #if ADEVERTISMENT
        if(advertisingMode){
            MenuAdvertisment.blit();
        }
        #endif
        SDL_RenderPresent(app.renderer);

        SDL_Delay( 1000/drawFPS );    // Delaying constant time between ticks to decrease CPU loading
    }
    // Clearing animations
    #if ADEVERTISMENT
    if(advertisingMode){
        MenuAdvertisment.clear();
    }
    #endif

    // Resetting values
    winning = false;
    loosing = false;
    skipping = false;

    flagedMines = 0;
    realOpenMines = 0;

    // Starting playing main game theme
    Mix_PlayMusic( Musics[MUS_MAIN_THEME], -1 );
    
    screenTimer = SDL_GetTicks64();  // Getting start time
}