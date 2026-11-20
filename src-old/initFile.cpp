#include "include.hpp"
#include <fstream>
#include "define.hpp"
#include "structs.hpp"

#include "initFile.hpp"
#include "pause.hpp"

// Loading initialasing settings in game
void loadInitFile(){
    // Reading file
    std::ifstream in(SETTING_FILE); // Open file to read
    std::string line;  // Output string line

    // Setting standart values for variables
    language = LNG_ENGLISH;
    MusicVolume = MIX_MAX_VOLUME/2;
    EffectsVolume = MIX_MAX_VOLUME/2;
    drawFPS = 60;
    mineX = 16;
    mineY = 20;
    mineCount = 40;
    leftTimer = 0;

    // Reading file until it end
    while(std::getline(in, line)){  
        std::string first = line.substr(0, line.find('=')-1);
        // Switching between options
        if( first == "language" ){
            std::string lang = line.substr(line.rfind('=')+2);
            if(lang == "russian"){
                language = LNG_RUSSIAN;
            }
            else if(lang == "english"){
                language = LNG_ENGLISH;
            }
        }
        else if( first == "music" ){
            MusicVolume = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "effects" ){
            EffectsVolume = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "max FPS" ){
            drawFPS = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "width" ){
            mineX = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "height" ){
            mineY = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "count" ){
            mineCount = std::stoi( line.substr(line.rfind('=')+2) );
        }
        else if( first == "playtime" ){ 
            leftTimer = std::stoi( line.substr(line.rfind('=')+2) );
        }
    }
    // Checking grid for getting over screen
    if(mineX > 60){
        mineX = 60;
    }
    // Checking grid for getting over screen
    if(mineY > 30){
        mineY = 30;
    }
    // Checking grid for getting over screen
    if(mineCount > mineX * mineY - 9){
        mineCount = mineX * mineY - 9;
    }

    in.close();  // Closing reading file
}

// Saving initialasing file
void saveInitFile(){
    std::ofstream setting(SETTING_FILE);  // Creating output file

    // Writing data to output
    setting << "# Language type (english/russian):" << std::endl;  // Extra comment
    switch (language)  // Writing language
    {
    case LNG_ENGLISH:
    default:
        setting << "language = english" << std::endl;
        break;
    case LNG_RUSSIAN:
        setting << "language = russian" << std::endl;
        break;
    }
    setting << std::endl << "# Technical part:" << std::endl;                 // Extra comment
    setting << "music = " << std::to_string(MusicVolume) << std::endl;        // Writing music volume
    setting << "effects = " << std::to_string(EffectsVolume) << std::endl;    // Writing effects volume
    setting << "max FPS = " << std::to_string(drawFPS) << std::endl;          // Writing frames per seconds

    setting << std::endl <<  "# Sizes of field:" << std::endl;                // Extra comment
    setting << "width = " << std::to_string(mineX) << std::endl;              // Writing width of grid in mines
    setting << "height = " << std::to_string(mineY) << std::endl;             // Writing height of grid in mines

    setting << std::endl << "# Count of mines on game field" << std::endl;    // Extra comment
    setting << "count = " << std::to_string(mineCount) << std::endl;          // Writing number of mines

    setting << std::endl << "# Time to solve puzzle, leave 0 if not enable" << std::endl;  // Extra comment
    setting << "playtime = " << std::to_string(leftTimer) << std::endl;       // Writing time for fully solving

    setting.close();  // Closing file
}