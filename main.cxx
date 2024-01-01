#include "SDL.hxx"

#include <cstdlib>

int main(int argc, char** argv) try
{
  SDL sdl{};

  return EXIT_SUCCESS;
} catch (std::exception const& ex) {
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", ex.what());
  return EXIT_FAILURE;
}
