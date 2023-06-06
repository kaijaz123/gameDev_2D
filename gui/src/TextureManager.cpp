#include "TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

// The render sequence is following in this
// 1. load the surface (i.e. an image) IMG_LOAD
// 2. Create texture from the surface SDL_CreateTextureFromSurface
// 3. Free the surface since we already have the texture created SDL_FreeSurface
// 4. Copy the texture into current rendering target SDL_RenderCopy
// 5. Render the result SDL_RenderPresent