// Responsible for parsing commands from the user into internal commands
module;
#include <unordered_map>
#include <string>
#include <optional>
#include <expected>
export module jork.parser;

export namespace jork {
    enum class command_type {
        help,

        north, south, east, west,
        up, down, in, out,

        look,
        examine,
        take,
        drop,
        inventory,
        wait,

        pet,

        unknown
    };
    struct command {
        command(command_type type, std::optional<std::string> object)
            : type(type), object(object) {}

        command_type type;
        std::optional<std::string> object;
    };

    std::expected<command, std::string> parse_command(std::string command);
}