#include "include.h"
#include "define.h"
#include "pause.h"

// 
static bool disableUpdate;  // Flag of disabling updating timer
static count saveTime;      // Value of time, saved for 

// Types of selected box
enum{  
    NORMAL_BOX,
    MUSIC_SLIDER_BOX,
    EFFECT_SLIDER_BOX,
    ENGLISH_BOX,
    RUSSIAN_BOX,
    ESCAPE_BOX
} SELCTED_BOX_types;


// Global GUI
#if TXT_count
staticText texts[TXT_count] = {
    [TXT_START_KEYS] = {.text = "Press on field to start\nНажмите на поле для старта\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.08, .aligment = MIDLE_text, .color = WHITE},
    [TXT_MINE_REST] = {.text = "Rest: %\nОсталось: %\n", 
        .fontSize = 24, .posX = 0.02, .posY = 0.02, .aligment = LEFT_text, .color = WHITE},
    [TXT_TIME] = {.text = "Time: %\nВремя: %\n", 
        .fontSize = 24, .posX = 0.9, .posY = 0.02, .aligment = RIGHT_text, .color = WHITE},
    [TXT_END_WIN] = {.text = "You win!\nВы выиграли!\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.06, .aligment = MIDLE_text, .color = WHITE},
    [TXT_END_LOOSE] = {.text = "You loose...\nВы проиграли...\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.06, .aligment = MIDLE_text, .color = WHITE},
    [TXT_END_SKIP] = {.text = "You stop game.\nВы остановили игру.\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.06, .aligment = MIDLE_text, .color = WHITE},
    [TXT_END_KEYS] = {.text = "Press any button.\nНажмите на любую кнопку.\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.11, .aligment = MIDLE_text, .color = WHITE},
    [TXT_PAUSE_PAUSE] = {.text = "Game on pause\nИгра на паузе\n", 
        .fontSize = 30, .posX = 0.5, .posY = 0.05, .aligment = MIDLE_text, .color = GREY},
    [TXT_PAUSE_MUSIC] = {.text = "Music:\nМузыка:\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.4, .aligment = MIDLE_text, .color = GREY},
    [TXT_PAUSE_SOUND] = {.text = "Sounds:\nЗвуки:\n", 
        .fontSize = 24, .posX = 0.5, .posY = 0.55, .aligment = MIDLE_text, .color = GREY},

};
#endif

Button esc = {.posX = 0.97, .posY = 0.03, .textureIndex = IMG_MENU_PAUSE};


void updateTranslation(enum LNG_types language){
    // Setting window title
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
        updateStaticText(texts + i, 0);
    }
    
    updateStaticText(texts + TXT_MINE_REST, mineCount - flagedMines);

    // Updating dinamic texts
    if(!disableUpdate){
        
        // Updating timer value, depend, if not equal 0 - rest time
        if(leftTimer){
            updateStaticText(texts + TXT_TIME, leftTimer - (SDL_GetTicks64() - screenTimer) / 1000);
        }
        else{
            updateStaticText(texts + TXT_TIME, (SDL_GetTicks64() - screenTimer) / 1000);
        }
    }
    else{
        updateStaticText(texts + TXT_TIME, saveTime);
    }
}

// Pause menu
void pause(){
    // Save time, when pause start
    Uint64 saveTimer = SDL_GetTicks64();

    // Creating pause text
    Slider musicSlider = {.textureButton = IMG_MENU_SCROLLER_BUTTON, .textureLine = IMG_MENU_SCROLLER_LINE, .posY = 0.46};
    Slider soundSlider = {.textureButton = IMG_MENU_SCROLLER_BUTTON, .textureLine = IMG_MENU_SCROLLER_LINE, .posY = 0.61};
    Button BtnFlagUSA = {.textureIndex = IMG_FLAG_USA, .posX = 0.3, .posY = 0.2};
    Button BtnFlagRUS = {.textureIndex = IMG_FLAG_RUS, .posX = 0.7, .posY = 0.2};

    // Initialasing GUI
    initSlider(&musicSlider);
    initSlider(&soundSlider);
    initButton(&BtnFlagUSA);
    initButton(&BtnFlagRUS);

    SDL_Event event;
    bool waiting = true;
    bool MouseDown = false;
    char inBox = NORMAL_BOX;
    enum LNG_types newLanguage = language;
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
            if(checkInSlider(&musicSlider, MouseX, MouseY)){
                inBox = MUSIC_SLIDER_BOX;
            }
            else if(checkInSlider(&soundSlider, MouseX, MouseY)){
                inBox = EFFECT_SLIDER_BOX;
            }
            else if(checkInButton(&BtnFlagUSA, MouseX, MouseY)){
                inBox = ENGLISH_BOX;
            }
            else if(checkInButton(&BtnFlagRUS, MouseX, MouseY)){
                inBox = RUSSIAN_BOX;
            }
            else if(checkInButton(&esc, MouseX, MouseY)){
                inBox = ESCAPE_BOX;
            }
            else{
                inBox = NORMAL_BOX;
            }
        }

        switch(inBox)
        {
        case MUSIC_SLIDER_BOX:  // If touch music slider
            MusicVolume = (MouseX - musicSlider.destLine.x) / 2;
            if(MouseX - musicSlider.destLine.x < 0) MusicVolume = 0;
            if(MouseX - musicSlider.destLine.x > 255) MusicVolume = 255;
            Mix_VolumeMusic(MusicVolume);  // Setting volume of music
            break;
        case EFFECT_SLIDER_BOX:  // If touch effects slider
            EffectsVolume = (MouseX - soundSlider.destLine.x) / 2;
            if(MouseX - soundSlider.destLine.x < 0) EffectsVolume = 0;
            if(MouseX - soundSlider.destLine.x > 255) EffectsVolume = 255;
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
        blitStaticText(texts + TXT_PAUSE_PAUSE);
        blitStaticText(texts + TXT_PAUSE_PAUSE);
        blitStaticText(texts + TXT_PAUSE_MUSIC);
        blitStaticText(texts + TXT_PAUSE_SOUND);
        // Drawing sliders
        blitSlider(&musicSlider, MusicVolume * 2);
        blitSlider(&soundSlider, EffectsVolume * 2);
        // Drawing buttons
        blitButton(&esc);
        blitButton(&BtnFlagUSA);
        blitButton(&BtnFlagRUS);

        // Blitting textures on screen
        SDL_RenderPresent(app.renderer);  

        // Delaying time to decrease CPU loading
        SDL_Delay(1000 / drawFPS);  
    }
    // Resetting timer to save rest time
    screenTimer += (SDL_GetTicks64() - saveTimer);

    // Updating dinamic texts
    if(!disableUpdate){
        // Updating timer value, depend, if not equal 0 - rest time
        if(leftTimer){
            updateStaticText(texts + TXT_TIME, leftTimer - (SDL_GetTicks64() - screenTimer) / 1000);
        }
        else{
            updateStaticText(texts + TXT_TIME, (SDL_GetTicks64() - screenTimer) / 1000);
        }
    }
    else{
        updateStaticText(texts + TXT_TIME, saveTime);
    }
};

void endMenu(){
    // Opening all cells to show correction
    fieldOpeanAll();

    // Stopping palying music, while waiting for restart
    Mix_PauseMusic();

    // Disabling update, while waiting for start
    disableUpdate = true;
    if(leftTimer){
        saveTime = leftTimer - (SDL_GetTicks64() - screenTimer) / 1000;
    }
    else{
        saveTime = (SDL_GetTicks64() - screenTimer) / 1000;
    }

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
                if(checkInButton(&esc, MouseX, MouseY)){  // Clicking on escape button
                    pause();
                }
                break;
            }
        }
        // Drawing
        SDL_RenderClear(app.renderer);

        // Drawing, what rest from field
        fieldBlit();

        // Drawing signs
        blitStaticText(texts + TXT_MINE_REST);
        blitStaticText(texts + TXT_TIME);

        // Drawing extra text with loosing/winning/skipping
        if(loosing){
            blitStaticText(texts + TXT_END_LOOSE);
        }
        else if(winning){
            blitStaticText(texts + TXT_END_WIN);
        }
        else if(skipping){
            blitStaticText(texts + TXT_END_SKIP);
        }
        blitStaticText(texts + TXT_END_KEYS);

        blitButton(&esc);

        SDL_RenderPresent(app.renderer);

        // Delaying constant time between ticks to decrease CPU loading
        SDL_Delay( 1000/drawFPS );    
    }
    fieldReset();

    // Resuming playing menu music
    Mix_PlayMusic( Musics[MUS_MENU_THEME], -1 );
    Mix_ResumeMusic();
}

void startMenu(){
    // Updating score
    saveTime = 0;
    updateStaticText(texts + TXT_MINE_REST, 0);
    updateStaticText(texts + TXT_TIME, saveTime);
    disableUpdate = true;

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
                if(checkInButton(&esc, MouseX, MouseY)){  // Clicking on escape button
                    pause();
                }
                else if(MouseY > HEADLINE){
                    // Generating new field
                    fieldGenerate(mineCount, mineX, mineY, MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
                    // First click
                    fieldLMBClick(MouseX / MINE_SIDE, (MouseY - HEADLINE) / MINE_SIDE);
                    waiting = false;
                }
                break;
            }
        }
        // Drawing
        SDL_RenderClear(app.renderer);

        fieldBlit();
        // Drawing signes after bacground
        blitStaticText(texts + TXT_START_KEYS);

        blitStaticText(texts + TXT_MINE_REST);
        blitStaticText(texts + TXT_TIME);

        blitButton(&esc);

        SDL_RenderPresent(app.renderer);

        SDL_Delay( 1000/drawFPS );    // Delaying constant time between ticks to decrease CPU loading
    }

    // Resetting values
    winning = false;
    loosing = false;
    skipping = false;

    flagedMines = 0;
    realOpenMines = 0;

    disableUpdate = false;

    // Starting playing main game theme
    Mix_PlayMusic( Musics[MUS_MAIN_THEME], -1 );
    
    screenTimer = SDL_GetTicks64();  // Getting start time
}