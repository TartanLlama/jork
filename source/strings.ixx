// String tables, to allow translation later
module;
#include <string>
#include <unordered_map>
export module jork.strings;

export namespace jork {
    namespace strings {
        enum class language {
            english
        };

        struct string_key {
            operator const char* () { return key; }
            const char* key;
        };

        extern language g_lang;
        extern const std::unordered_map<const char*, const char*> g_english_translations;

        inline std::string loc(string_key key) {
            if (g_lang == language::english) return g_english_translations.at(key);
        }

        // All string keys to use in the rest of the program go here
        // To add one easily run `python tools/add_string.py <key> <english string>`
        inline string_key window_name{ "window_name" };
        inline string_key wrench_name{ "wrench_name" };
        inline string_key wrench_inventory{ "wrench_inventory" };
        inline string_key wrench_description{ "wrench_description" };
        inline string_key lower_yourself{ "lower_yourself" };
        inline string_key huge_hole { "huge_hole" };
        inline string_key wrench_lies { "wrench_lies" };
        inline string_key dangling_from_rappel_rope { "dangling_from_rappel_rope" };
        inline string_key you_take_the_wrench { "you_take_the_wrench" };
        inline string_key already_taken_wrench { "already_taken_wrench" };
        inline string_key cannot_see_item { "cannot_see_item" };
        inline string_key item_does_not_exist { "item_does_not_exist" };
        inline string_key pet_marshmallow { "pet_marshmallow" };
        inline string_key cannot_pet_that { "cannot_pet_that" };
    }
}