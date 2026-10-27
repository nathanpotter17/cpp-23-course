#pragma once

#include "includes/operations.h"
#include "includes/types.h"

#include <charconv>
#include <cstddef>
#include <optional>
#include <string_view>
#include <system_error>

// --- Expressions -------------------------------------------------------------

// "lhs op rhs", with the operator already looked up in the table.
struct Expression {
    i32 lhs = 0;
    const Operation* operation = nullptr;
    i32 rhs = 0;

    Result evaluate() const {
        return operation->apply(lhs, rhs);
    }
};

// --- Parsing helpers ---------------------------------------------------------

// Drops leading spaces. npos ("no position") is what find_* returns on a miss.
inline std::string_view skip_spaces(std::string_view text) {
    const std::string_view::size_type start = text.find_first_not_of(' ');
    return start == std::string_view::npos ? std::string_view{} : text.substr(start);
}

// Reads an i32 from the front of text, and removes it from text.
// from_chars doesn't throw or allocate; it reports where it stopped and why.
inline std::optional<i32> take_number(std::string_view& text) {
    text = skip_spaces(text);

    i32 value = 0;
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);

    // No digits at all, or a number too big for an i32.
    if (error != std::errc{}) {
        return std::nullopt;
    }

    // Subtracting pointers gives a std::ptrdiff_t, a signed alias;
    // string_view sizes are std::size_t, an unsigned one.
    const std::ptrdiff_t consumed = end - text.data();
    text.remove_prefix(static_cast<std::size_t>(consumed));
    return value;
}

// --- The parser --------------------------------------------------------------

// Accepts "12 + 12", "-7*6" and so on: number, operator, number.
inline std::optional<Expression> parse(std::string_view text) {
    const std::optional<i32> lhs = take_number(text);
    if (!lhs) {
        return std::nullopt;
    }

    text = skip_spaces(text);
    if (text.empty()) {
        return std::nullopt;
    }

    const Operation* operation = find_operation(text.front());
    if (!operation) {
        return std::nullopt;
    }
    text.remove_prefix(1);

    const std::optional<i32> rhs = take_number(text);
    if (!rhs || !skip_spaces(text).empty()) {
        return std::nullopt;
    }

    return Expression{*lhs, operation, *rhs};
}
