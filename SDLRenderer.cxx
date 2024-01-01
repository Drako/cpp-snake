#include "SDLRenderer.hxx"

SDLRenderer::SDLRenderer(SDLWindow& window)
{
  renderer_ = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
  if (renderer_==nullptr) {
    throw SDLError("Failed to create renderer");
  }
  SDL_RenderSetVSync(renderer_, SDL_TRUE);
  SDL_Log("Created renderer successfully.");
}

SDLRenderer::~SDLRenderer() noexcept
{
  destroy();
}

SDLRenderer::SDLRenderer(SDLRenderer&& src) noexcept
    :renderer_{src.renderer_}
{
  src.renderer_ = nullptr;
}

SDLRenderer& SDLRenderer::operator=(SDLRenderer&& src) noexcept
{
  destroy();
  std::swap(renderer_, src.renderer_);
  return *this;
}

void SDLRenderer::destroy() noexcept
{
  if (renderer_!=nullptr) {
    SDL_DestroyRenderer(renderer_);
    SDL_Log("Destroyed renderer successfully.");
    renderer_ = nullptr;
  }
}
