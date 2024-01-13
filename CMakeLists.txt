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
FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED COMPONENTS serialization program_options)

add_executable(Snake WIN32 MACOSX_BUNDLE
    main.cxx
    SDL.cxx SDL.hxx
    SDLWindow.cxx SDLWindow.hxx
    SDLRenderer.cxx SDLRenderer.hxx
    game/GameState.hxx
    game/GameStateManager.cxx game/GameStateManager.hxx
    game/LoadingState.cxx game/LoadingState.hxx
    game/SplashState.cxx game/SplashState.hxx
    game/MenuState.cxx game/MenuState.hxx
    game/PlayingState.cxx game/PlayingState.hxx
    game/GameOverState.cxx game/GameOverState.hxx
    game/HighScoreState.cxx game/HighScoreState.hxx
    game/CreditsState.cxx game/CreditsState.hxx
    game/AssetManager.cxx game/AssetManager.hxx
    game/ui/Button.cxx game/ui/Button.hxx
    game/ui/LineInput.cxx game/ui/LineInput.hxx
    game/ui/UiColor.hxx game/ui/UiColor.cxx
    game/HighScoreManager.cxx game/HighScoreManager.hxx
)

file(GLOB assets assets/*)
target_sources(Snake PRIVATE ${assets})
set_source_files_properties(${assets} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/assets)

target_link_libraries(Snake PRIVATE
    SDL2::SDL2 SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    Boost::headers Boost::serialization Boost::program_options
)