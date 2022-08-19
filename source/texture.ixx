// Helper for dealing with SDL textures
module;
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string_view>
export module jork.texture;

export namespace jork {
	class texture
	{
	public:
		texture(SDL_Renderer* renderer, TTF_Font* font) :
			renderer_(renderer), font_(font) {}
		~texture() { free(); }

		bool load_from_file(std::string path);
		bool load_from_rendered_text(std::string_view texture_text, SDL_Color textColor);
		void free();
		void set_colour(Uint8 red, Uint8 green, Uint8 blue);

		void set_blend_mode(SDL_BlendMode blending);
		void set_alpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		bool loaded() { return texture_ != nullptr; }
		int width() { return width_; }
		int height() { return height_; }
		int lines_needed() {
			auto row_height = TTF_FontHeight(font_);
			return height_ / row_height;
		}

	private:
		SDL_Texture* texture_ = nullptr;
		SDL_Renderer* renderer_ = nullptr;
		TTF_Font* font_ = nullptr;

		int width_ = 0;
		int height_ = 0;
	};
}