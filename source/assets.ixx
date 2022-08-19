// Helpers for working with asset files
module;
#include <filesystem>
#include <string_view>
export module jork.assets;

export namespace jork {
    inline auto get_asset_path(std::string_view filename) {
        std::filesystem::path asset_path = "assets";
        return asset_path / filename;
    }
}