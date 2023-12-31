cmake_minimum_required(VERSION 3.10)

project(minesweeper)  # Main game project

# Set source files
add_executable( ${CMAKE_PROJECT_NAME}
    # Base includes and defines
    src/include.hpp
    src/define.hpp
    src/structs.hpp

    # Function of initialasing all data
    src/init.hpp
    src/init.cpp

    # Functions of loading all in-game data
    src/dataLoader.hpp
    src/dataLoader.cpp

    # Base interface
    src/baseGUI.hpp
    src/baseGUI.cpp

    # Interface of game pausing
    src/pause.hpp
    src/pause.cpp

    # System of initialasing file loading and unloading
    src/initFile.hpp
    src/initFile.cpp

    # In game classes
    src/entity.hpp
    src/entity.cpp

    # Main function
    src/main.cpp
    
    # Adding game icone
    src/set.rc
)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 -mwindows ${CMAKE_CSS_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Including external libraries
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Finding place of external libraries
# Including SDL-family
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

# Including libzip
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/share")
find_package(libzip CONFIG REQUIRED)

# Including external included directories
include_directories(${SDL2_INCLUDE_DIRS})

# Including linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image mingw32)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf mingw32)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer mingw32)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${libzip_LIBRARIES} libzip::zip)