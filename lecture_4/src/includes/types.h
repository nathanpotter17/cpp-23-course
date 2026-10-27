#pragma once

#include <concepts>
#include <cstdint>
#include <optional>
#include <type_traits>

// --- Two spellings, one meaning ----------------------------------------------

// C's typedef: the new name sits where a variable's name would.
typedef std::int32_t i32;

// C++11's alias declaration: the name on the left, the type on the right.
// Redeclaring an alias is allowed when it names the same type, so this line
// compiling proves both spellings mean exactly the same thing.
using i32 = std::int32_t;

using i64 = std::int64_t;
using u32 = std::uint32_t;

// An alias is another name, not a new type: the compiler can't tell them apart.
static_assert(std::is_same_v<i32, std::int32_t>);
static_assert(sizeof(i32) == 4 && sizeof(i64) == 8);

// --- Aliases for standard library types --------------------------------------

// A calculation either produces an i32 or fails (overflow, division by zero).
using Result = std::optional<i32>;

// --- Function pointer aliases ------------------------------------------------

// The typedef spelling hides the name inside the type:
//     typedef Result (*BinaryOp)(i32, i32);
using BinaryOp = Result (*)(i32, i32);

// --- Alias templates ---------------------------------------------------------

// The standard library's _t helpers are alias templates too:
// std::make_unsigned_t<T> is short for typename std::make_unsigned<T>::type.
template <std::integral T>
using Unsigned = std::make_unsigned_t<T>;

static_assert(std::is_same_v<Unsigned<i32>, u32>);

// The same bits, read as unsigned. -1 becomes 0xffffffff, so hex and binary
// output shows the two's complement pattern instead of a minus sign.
template <std::integral T>
constexpr Unsigned<T> bits(T value) {
    return static_cast<Unsigned<T>>(value);
}
