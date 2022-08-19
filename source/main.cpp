import jork.assets;
import jork.console;
import jork.constants;
import jork.parser;
import jork.strings;
import jork.texture;
import jork.world;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <ranges>
#include <stdio.h>
#include <string>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace jork;

class game {
public:
    enum class state {
        main_menu,
        normal_play
    };

    static game create();

    game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
    ~game();

    std::string do_down();
    std::string do_look();
    std::string do_take(std::string object);
    std::string do_drop(std::string object);
    std::string do_inventory();
    std::string do_help(std::string object);
    std::string do_pet(std::string object);

    std::string handle_command(std::string input);

    void loop();

private:
    SDL_Window* window_ = nullptr;
    SDL_Surface* screen_surface_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
    state state_ = state::main_menu;
    world world_;
    console console_;
    std::optional<texture> menu_texture_;
};

game game::create() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        std::terminate();
    }

    auto window_name = loc(strings::window_name);
    auto window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        std::terminate();
    }

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        std::terminate();
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        std::terminate();
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        std::terminate();
    }

    auto font = TTF_OpenFont(get_asset_path("Retro Gaming.ttf").string().c_str(), font_size);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        std::terminate();
    }

    return game(window, renderer, font);
}

game::game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font)
    : window_(window), renderer_(renderer), font_(font), console_(renderer, font) {
    menu_texture_ = texture{ renderer_, font_ };

    SDL_StartTextInput();
}

std::string game::do_down() {
    world_.go_down();
    return loc(strings::lower_yourself);
}

std::string game::do_look() {
    if (world_.level() == 0) {
        std::string ret = loc(strings::huge_hole);
        return ret;
    }
    else {
        return loc(strings::dangling_from_rappel_rope);
    }
}

std::string game::do_take(std::string object) {
    return loc(strings::cannot_see_item);
}

std::string game::do_drop(std::string object) {
    auto obj_id = get_object_id(object);
    if (!obj_id) return loc(strings::item_does_not_exist);

    if (!world_.inventory().contains(*obj_id)) return "You do not have that item.";

    world_.remove_inventory_item(*obj_id);
    return "You drop it";
}

std::string game::do_inventory() {
    if (world_.inventory().size() == 0) return "You are not carrying anything.";
    std::string ret = "You are carrying:\n";
    for (auto&& [k, v] : world_.inventory()) {
        ret += "  " + v.inventory + "\n";
    }
    ret.pop_back();
    return ret;
}

std::string game::do_help(std::string area) {
    return R"(The following commands are available (short versions in parentheses):
  (n)orth, (e)ast, (s)outh, (w)est
  (u)p, (d)own
  in, out
  
  (l)ook, e(x)amine <object>,
  take <object>, drop <object>
  (i)nventory, wait (z))";
}

std::string game::do_pet(std::string object) {
    if ((object == "marshmallow" || object == "cat") && world_.level() == 0) {
        return loc(strings::pet_marshmallow);
    }
    return loc(strings::cannot_pet_that);
}

std::string game::handle_command(std::string input) {
    auto com = parse_command(std::move(input));

    if (!com) return com.error();

    using enum command_type;
    switch (com->type) {
    case down:
        return do_down();
    case north:
    case south:
    case east:
    case west:
    case up:
    case in:
    case out:
        return "You can only go down";
    case look:
        return do_look();
    case take:
        return do_take(*com->object);
    case drop:
        return do_drop(*com->object);
    case inventory:
        return do_inventory();
    case help:
        return do_help(*com->object);
    case pet:
        return do_pet(*com->object);
    case unknown:
        return "Unknown command";
    default:
        return "You did the thing";
    }
}

void game::loop() {
    SDL_Event e;
    SDL_Color text_colour = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (state_ == state::main_menu) {
        if (!menu_texture_->loaded()) {
            menu_texture_->load_from_rendered_text("You May Only Go Down\n\nPress return to begin.", text_colour);

            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer_);

            menu_texture_->render(200, 180);

            SDL_RenderPresent(renderer_);
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    menu_texture_->free();
                    console_.reset_view();
                    state_ = state::normal_play;
                }
            }
        }
    }

    if (state_ == state::normal_play) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) {
                    for (auto i = 0; i < e.wheel.y; ++i) {
                        console_.scroll_up();
                    }
                }
                else if (e.wheel.y < 0) {
                    for (auto i = 0; i < -e.wheel.y; ++i) {
                        console_.scroll_down();
                    }
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    console_.reset_view();
                    console_.backspace();
                }
                else if (e.key.keysym.sym == SDLK_RETURN) {
                    auto command = console_.commit();
                    auto response = handle_command(command);
                    if (response.back() != '\n') response += '\n';
                    console_.display_text(response);
                }
                else if (e.key.keysym.sym == SDLK_UP) {
                    console_.begin_scrolling(console::scroll_direction::up);
                }
                else if (e.key.keysym.sym == SDLK_DOWN) {
                    console_.begin_scrolling(console::scroll_direction::down);
                }
            }
            else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN) {
                    console_.stop_scrolling();
                }
            }
            else if (e.type == SDL_TEXTINPUT) {
                console_.input(e.text.text);
            }
        }

        console_.tick_scrolling();
        console_.render();
    }
}

game::~game() {
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

void game_loop(void* g) {
    static_cast<game*>(g)->loop();
}

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
int main() {
#else
int SDL_main(int argc, char** argv) {
#endif
    game g = game::create();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(game_loop, static_cast<void*>(&g), -1, true);
#else
    while (true) {
        g.loop();
    }
#endif
}