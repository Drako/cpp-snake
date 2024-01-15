cmake_minimum_required(VERSION 3.27)
project(Snake VERSION 1.0.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectHelpers)

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
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Boost_USE_DEBUG_LIBS ON)
else ()
  set(Boost_USE_DEBUG_LIBS OFF)
endif ()
find_package(Boost 1.74.0 REQUIRED COMPONENTS serialization program_options)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.hxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.hxx
    @ONLY
)

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
    ${CMAKE_CURRENT_BINARY_DIR}/Config.hxx
)

file(GLOB assets assets/*)
target_sources(Snake PRIVATE ${assets})

set_target_properties(Snake PROPERTIES
    RUNTIME_OUTPUT_NAME snake
)
target_link_libraries(Snake PRIVATE
    SDL2::SDL2 SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    Boost::headers Boost::serialization Boost::program_options
)

if (WIN32)
  copy_dependency_dll(TARGET Snake DEPENDENCY SDL2::SDL2)
  copy_dependency_dll(TARGET Snake DEPENDENCY SDL2_image::SDL2_image)
  copy_dependency_dll(TARGET Snake DEPENDENCY SDL2_ttf::SDL2_ttf)

  include(InstallRequiredSystemLibraries)
  install(IMPORTED_RUNTIME_ARTIFACTS SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

  install(TARGETS Snake DESTINATION bin)
  install(FILES ${assets} DESTINATION assets)

  set(CPACK_GENERATOR NSIS64)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "Draconic Bytes\\\\Snake")
  set(CPACK_PACKAGE_NAME "Snake")
  set(CPACK_PACKAGE_VENDOR "Draconic Bytes")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_EXECUTABLES "snake;Snake")
  include(CPack)
elseif (APPLE)
  set_source_files_properties(${assets} PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources/assets
  )
else ()
  install(TARGETS Snake DESTINATION bin)
  install(FILES ${assets} DESTINATION share/snake/assets)
endif ()
