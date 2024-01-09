cmake_minimum_required(VERSION 3.27)
project(Snake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG a7090e8ce184501cfc9e80afa6cafb5bfd3b371c # boost-1.74.0
    FIND_PACKAGE_ARGS 1.74.0 REQUIRED COMPONENTS serialization program_options system
)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG 15ead9a40d09a1eb9972215cceac2bf29c9b77f6 # release-2.28.5
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG 28b9daa15a59aa2829cd29944ca9ffbf049d7667 # release-2.8.1
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG 89d1692fd8fe91a679bb943d377bfbd709b52c23 # release-2.20.2
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf Boost)

add_executable(Snake WIN32
    main.cxx
    SDL.cxx SDL.hxx
    SDLWindow.cxx SDLWindow.hxx
    SDLRenderer.cxx SDLRenderer.hxx
    game/GameState.hxx
    game/GameStateManager.cxx game/GameStateManager.hxx
    game/LoadingState.cxx game/LoadingState.hxx
    game/SplashState.cxx game/SplashState.hxx
    game/MenuState.cxx game/MenuState.hxx
    game/DummyState.cxx game/DummyState.hxx
    game/AssetManager.cxx game/AssetManager.hxx
    game/ui/Button.cxx game/ui/Button.hxx
    game/ui/LineInput.cxx game/ui/LineInput.hxx
    game/ui/UiColor.hxx game/ui/UiColor.cxx
)

target_link_libraries(Snake PRIVATE
    SDL2::SDL2 SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    Boost::headers Boost::serialization Boost::program_options
)