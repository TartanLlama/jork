// A scrolling console implementation
module;
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <string>
export module jork.console;

import jork.texture;
import jork.constants;


export namespace jork {
    class console {
    public:
        console(SDL_Renderer* renderer, TTF_Font* font) :
            renderer_(renderer), font_(font), input_texture_(renderer, font) {
        }

        void backspace();
        std::string commit();
        void input(char* c);
        void display_text(std::string text);

        enum class scroll_direction {
            up, down, none
        };
        void begin_scrolling(scroll_direction dir);
        void stop_scrolling();
        void tick_scrolling();

        void scroll_up();
        void scroll_down();

        void reset_view();
        void render();

    private:
        struct text_entry {
            std::string text;
            int n_lines;
        };

        SDL_Renderer* renderer_ = nullptr;
        TTF_Font* font_ = nullptr;

        std::vector<text_entry> text_buffer_;

        //Used to track scroll positions
        decltype(text_buffer_)::iterator top_;
        decltype(text_buffer_)::iterator saved_top_;
        int saved_scroll_overflow_ = 0;
        int scroll_overflow_ = 0;

        scroll_direction scrolling_ = scroll_direction::none;

        std::string current_line_;
        texture input_texture_;
        bool needs_render_ = false;

        static inline const char* prompt_ = "> ";
    };
}