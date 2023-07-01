#include "Game.hpp"

class TextureManager {

public:
    // with static keywords means it can be called outside without an object (initialization)
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);

};