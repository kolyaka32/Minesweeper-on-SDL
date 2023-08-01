cmake_minimum_required(VERSION 3.10)

project(base)

add_executable(base
    src/main.cpp
)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)


target_include_directories(base
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
    PUBLIC ${SDL2_ttf_config_path}
)

target_link_libraries(base PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image mingw32)
target_link_libraries(base PUBLIC ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf mingw32)
