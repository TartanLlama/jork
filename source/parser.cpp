module;
#include <iterator>
#include <sstream>
#include <vector>
#include <expected>
#include <unordered_map>
#include <optional>
module jork.parser;
using namespace jork;

struct command_desc {
    command_desc(std::string l, std::string s, command_type c) :
        long_(std::move(l)), short_(std::move(s)), command_(c) {}

    command_desc(std::string l, command_type c) :
        long_(std::move(l)), short_(""), command_(c) {}
    std::string long_;
    std::string short_;
    command_type command_;
};

std::unordered_map<std::string, command_type> build_command_map(std::initializer_list<command_desc> descs) {
    std::unordered_map<std::string, command_type> ret;
    for (auto&& desc : descs) {
        ret[std::move(desc.long_)] = desc.command_;
        if (!desc.short_.empty()) {
            ret[std::move(desc.short_)] = desc.command_;
        }
    }

    return ret;
}

template <typename Out>
void split(const std::string& s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::expected<command, std::string> jork::parse_command(std::string cmd) {
    using enum command_type;
    static auto command_map = build_command_map({
        {"help", "h", help},
        {"north", "n", north},
        {"south", "s", south},
        {"east", "e", east},
        {"west", "w", west},
        {"up", "u", up},
        {"down", "d", down},
        {"in", in},
        {"out", out},
        {"look", "l", look},
        {"examine", "x", examine},
        {"take", take},
        {"drop", drop},
        {"inventory", "i", inventory},
        {"wait", "z", wait},
        {"pet", pet}
        });

    auto commands = split(cmd, ' ');

    if (commands.size() == 0) return command{ unknown, std::nullopt };

    if (command_map.contains(commands[0])) {
        auto c = command_map[commands[0]];

        switch (c) {
        case help:
            if (commands.size() == 2) {
                return command{ c, commands[1] };
            }
            else {
                return command{ c, std::string{nullptr, 1} };
            }
        case north:
        case south:
        case east:
        case west:
        case up:
        case down:
        case in:
        case out:
        case look:
        case inventory:
        case wait:
            if (commands.size() > 1)
                return std::unexpected("That command doesn't take an object");
            else
                return command{ c, std::nullopt };

        case examine:
        case take:
        case drop:
        case pet:
            if (commands.size() > 2) {
                return std::unexpected("That command only takes a single object");
            }
            else if (commands.size() == 1) {
                return std::unexpected("That command requires an object");
            }
            else {
                return command{ c, commands[1] };
            }
        }
    }

    return command{ unknown, std::nullopt };
}