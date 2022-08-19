// Manager for world state
module;
#include <string>
#include <unordered_map>
#include <optional>
export module jork.world;

import jork.strings;

export namespace jork {
    struct object {
        std::string name;
        std::string inventory;
        std::string examine;
    };

    enum class object_id {
        wrench
    };

    std::optional<object_id> get_object_id(std::string name) {
        if (name == "wrench") return object_id::wrench;
        return std::nullopt;
    }

    class world {
    public:
        void go_down() {
            level_++;
        }
        int level() const {
            return level_;
        }
        bool wrench_taken() const {
            return wrench_taken_;
        }
        void take_wrench() {
            wrench_taken_ = true;
            inventory_[object_id::wrench] = object{ loc(strings::wrench_name), loc(strings::wrench_inventory), loc(strings::wrench_description) };
        }
        void put_back_wrench() {
            wrench_taken_ = false;
            remove_inventory_item(object_id::wrench);
        }
        void remove_inventory_item(object_id obj) {
            inventory_.erase(obj);
        }
        auto const& inventory() {
            return inventory_;
        }
    private:
        int level_ = 0;
        bool wrench_taken_ = false;
        std::unordered_map<object_id, object> inventory_;
    };
}