cmake_minimum_required(VERSION 3.27)
project(Snake VERSION 1.2.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectHelpers)

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG 147a8ee32dbf9ac02f3794964490687b6bbda1bc # release-3.4.14
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG bec9134a26c7d0f31b36d6083c25296e04cabff5 # release-3.4.4
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG a1ce3670aec736ecbf0936c43f2f0cc53aa61e5b # release-3.2.2
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.hxx.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Config.hxx"
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
    game/TranslationManager.cxx game/TranslationManager.hxx
    NonCopyable.hxx
)

file(GLOB assets assets/*)
target_sources(Snake PRIVATE ${assets})

if (WIN32)
  target_sources(Snake PRIVATE windows-resources.rc)
endif (WIN32)

if ((NOT WIN32) AND (NOT APPLE))
  set_target_properties(Snake PROPERTIES
      RUNTIME_OUTPUT_NAME snake
  )
endif ()
target_link_libraries(Snake PRIVATE
    SDL3::SDL3-shared
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if (WIN32)
  #copy_dependency_dll(TARGET Snake DEPENDENCY SDL3-shared)
  #copy_dependency_dll(TARGET Snake DEPENDENCY SDL3_image::SDL3_image)
  #copy_dependency_dll(TARGET Snake DEPENDENCY SDL3_ttf::SDL3_ttf)

  include(InstallRequiredSystemLibraries)
  #install(IMPORTED_RUNTIME_ARTIFACTS SDL3-shared SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

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
  target_sources(Snake PRIVATE snake-icon.icns)
  set_source_files_properties(snake-icon.icns PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )

  set_target_properties(Snake PROPERTIES
      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Developer"
      XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
  )

  get_dependency_location(SDL3_directory DEPENDENCY SDL3::SDL3)
  # TODO: Fix version
  # string(REPLACE "Cellar/sdl2/2.28.5/lib" "lib" dependency_dir "${SDL3_directory}")

  configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/FixupBundle.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/FixupBundle.cmake"
      @ONLY
  )
  install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/FixupBundle.cmake")

  set(MACOSX_BUNDLE_BUNDLE_NAME "Snake")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2024, Felix Bytow &lt;drako@drako.guru&gt;")
  set(MACOSX_BUNDLE_ICON_FILE snake-icon.icns)
else ()
  configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/Snake.desktop.in"
      "${CMAKE_CURRENT_BINARY_DIR}/Snake.desktop"
      @ONLY
  )

  install(TARGETS Snake DESTINATION bin)
  install(FILES ${assets} DESTINATION share/snake/assets)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Snake.desktop" DESTINATION share/applications)
endif ()
