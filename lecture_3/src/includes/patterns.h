#pragma once

#include <string_view>

// --- Starting patterns -------------------------------------------------------

// C arrays: the element count comes from the initializer. inline lets every
// file that includes this header share one array.
namespace patterns {

inline constexpr std::string_view block[] = {
    "##",
    "##",
};

inline constexpr std::string_view blinker[] = {
    "###",
};

inline constexpr std::string_view glider[] = {
    ".#.",
    "..#",
    "###",
};

} // namespace patterns
