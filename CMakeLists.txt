cmake_minimum_required(VERSION 3.27)
project(Snake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG 15ead9a40d09a1eb9972215cceac2bf29c9b77f6 # release-2.28.5
    FIND_PACKAGE_ARGS NAMES SDL2
)
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG 28b9daa15a59aa2829cd29944ca9ffbf049d7667 # release-2.8.1
    FIND_PACKAGE_ARGS NAMES SDL2_image
)
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG 89d1692fd8fe91a679bb943d377bfbd709b52c23 # release-2.20.2
    FIND_PACKAGE_ARGS NAMES SDL2_ttf
)
FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

add_executable(Snake WIN32 main.cxx
    SDL.cxx
    SDL.hxx)

target_link_libraries(Snake PRIVATE
    SDL2::SDL2 SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)