module;
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
module jork.texture;
using namespace jork;

bool texture::load_from_file(std::string path)
{
    //Get rid of preexisting texture
    free();

    SDL_Texture* new_texture = nullptr;

    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

        new_texture = SDL_CreateTextureFromSurface(renderer_, loaded_surface);
        if (new_texture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            width_ = loaded_surface->w;
            height_ = loaded_surface->h;
        }

        SDL_FreeSurface(loaded_surface);
    }

    //Return success
    texture_ = new_texture;
    return texture_ != nullptr;
}

bool texture::load_from_rendered_text(std::string_view texture_text, SDL_Color text_colour)
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font_, texture_text.data(), text_colour, 640);
    if (text_surface != nullptr)
    {
        //Create texture from surface pixels
        texture_ = SDL_CreateTextureFromSurface(renderer_, text_surface);
        if (texture_ == nullptr)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            width_ = text_surface->w;
            height_ = text_surface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(text_surface);
    }
    else
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    return texture_ != nullptr;
}

void texture::free()
{
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
        width_ = 0;
        height_ = 0;
    }
}

void texture::set_colour(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(texture_, red, green, blue);
}

void texture::set_blend_mode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(texture_, blending);
}

void texture::set_alpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture_, alpha);
}

void texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect render_quad = { x, y, width_, height_ };

    //Set clip rendering dimensions
    if (clip != nullptr)
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(renderer_, texture_, clip, &render_quad, angle, center, flip);
}