#pragma once

#include "includes/image.h"

#include <nlohmann/json.hpp>

#include <string>

// One gradient image described in the JSON config.
struct Swatch {
    std::string name;
    int width = 0;
    int height = 0;
    Rgb from{};
    Rgb to{};
};

// Generates to_json/from_json, so json.get<Swatch>() and json(swatch) just work.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Swatch, name, width, height, from, to)
