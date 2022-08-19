module jork.strings;
jork::strings::language jork::strings::g_lang = jork::strings::language::english;

const std::unordered_map<const char*, const char*> jork::strings::g_english_translations{
    {strings::window_name, "You Can Only Go Down"},

    { strings::wrench_name, "wrench" },
    { strings::wrench_inventory, "A wrench" },
    { strings::wrench_description, "A wrench for repairing bolts." },

    { strings::lower_yourself, "You lower yourself down the rope."},
    { strings::huge_hole, "You are in front of a huge hole in the ground with a rope dangling downwards. Marshmallow the cat sits next to it." },
    { strings::wrench_lies, "The repair wrench lies next to the rope support." },
    { strings::dangling_from_rappel_rope, "You are dangling from the rappel rope. There is a bolt in front of you." },
    { strings::you_take_the_wrench, "You take the wrench. It feels cold and heavy against your palm." },
    { strings::already_taken_wrench, "You have already taken the wrench." },
    { strings::cannot_see_item, "You can't see that item here." },
    { strings::item_does_not_exist, "That item does not exist." },
    { strings::pet_marshmallow, "You pet Marshmallow on his little head. He looks pleased" },
    { strings::cannot_pet_that, "You cannot pet that" },
};