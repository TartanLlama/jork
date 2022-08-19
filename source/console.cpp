module;
#include <sstream>
#include <SDL2/SDL_ttf.h>
#include <ranges>
module jork.console;

void jork::console::backspace() {
    if (current_line_.size() > 0) {
        current_line_.pop_back();
        needs_render_ = true;
    }
}

std::string jork::console::commit() {
    text_buffer_.push_back({ prompt_ + current_line_ + '\n', 1 });
    return std::exchange(current_line_, "");
}

void jork::console::display_text(std::string text) {
    SDL_Color text_colour = { 0xFF, 0xFF, 0xFF, 0xFF };
    input_texture_.load_from_rendered_text(text.c_str(), text_colour);
    auto lines_needed = input_texture_.lines_needed();
    text_buffer_.push_back({ text, lines_needed });
    text_buffer_.push_back({ "\n", 1 });
    reset_view();
}

void jork::console::begin_scrolling(scroll_direction dir) {
    scrolling_ = dir;
}

void jork::console::stop_scrolling() {
    scrolling_ = scroll_direction::none;
}

void jork::console::tick_scrolling() {
    if (scrolling_ == scroll_direction::up) {
        scroll_up();
    }
    else if (scrolling_ == scroll_direction::down) {
        scroll_down();
    }
}

void jork::console::input(char* c) {
    reset_view();
    auto max_chars = screen_width / TTF_FontHeight(font_);
    if (current_line_.size() < max_chars) {
        current_line_ += c;
    }
    needs_render_ = true;
}

void jork::console::reset_view() {
    // Initialize
    if (text_buffer_.empty()) {
        top_ = end(text_buffer_);
        saved_top_ = top_;
        saved_scroll_overflow_ = 0;
        needs_render_ = true;
        return;
    }

    //Walk backwards until we either hit the start,
    //or we have walked off the top of the screen
    auto start = end(text_buffer_) - 1;
    auto max_lines = screen_height / TTF_FontHeight(font_) - 1;
    auto lines_seeked = 0;
    while (start != begin(text_buffer_) && lines_seeked < max_lines) {
        --start;
        lines_seeked += start->n_lines;
    }

    //This tracks the number of additional lines which were seeked over
    scroll_overflow_ = lines_seeked > max_lines ? lines_seeked - max_lines : 0;
    top_ = start;
    saved_top_ = start;
    saved_scroll_overflow_ = scroll_overflow_;

    needs_render_ = true;
}

void jork::console::scroll_up() {
    //If there are overflow lines, scroll to the next highest one
    if (scroll_overflow_ > 0) {
        --scroll_overflow_;
        needs_render_ = true;
    }
    //Otherwise, so long as we don't go all the way off the top,
    //scroll up one entry
    else if (top_ != begin(text_buffer_)) {
        --top_;
        scroll_overflow_ = top_->n_lines - 1;
        needs_render_ = true;
    }
}

void jork::console::scroll_down() {
    //If this entry has overflow lines, scroll to the next one
    if (scroll_overflow_ < top_->n_lines - 1) {
        //Ensure we don't scroll beyond the bottom of the screen
        if (top_ != saved_top_ || scroll_overflow_ != saved_scroll_overflow_) {
            ++scroll_overflow_;
            needs_render_ = true;
        }
    }
    //Otherwise, scroll to the entry below
    else if (top_ != saved_top_) {
        ++top_;
        scroll_overflow_ = 0;
        needs_render_ = true;
    }
}

void jork::console::render() {
    SDL_Color text_colour;

    if (needs_render_) {
        auto start = top_;
        auto max_lines = screen_height / TTF_FontHeight(font_);
        auto end = std::end(text_buffer_) - start < max_lines ? std::end(text_buffer_) : top_ + max_lines;

        auto joined = std::ranges::subrange(start, end)
            | std::views::transform([](auto entry) { return entry.text; })
            | std::views::join
            | std::views::common;
        std::string text(joined.begin(), joined.end());
        text += prompt_ + current_line_;
        input_texture_.load_from_rendered_text(text.c_str(), text_colour);

        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer_);

        //If there are overflow lines, cut them off the top
        input_texture_.render(0, -scroll_overflow_ * TTF_FontHeight(font_));

        SDL_RenderPresent(renderer_);
    }

    needs_render_ = false;
}