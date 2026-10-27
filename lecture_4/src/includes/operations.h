#pragma once

#include "includes/types.h"

#include <algorithm>
#include <array>
#include <string_view>
#include <type_traits>
#include <utility>

// --- Checked arithmetic ------------------------------------------------------

// Every i32 operation is done in i64, where it can't overflow, and the result
// is kept only if it fits back into an i32.
constexpr Result narrow(i64 wide) {
    if (!std::in_range<i32>(wide)) {
        return std::nullopt;
    }

    return static_cast<i32>(wide);
}

// --- Operator symbols --------------------------------------------------------

// An enum class with a fixed underlying type: every Symbol is stored as a char.
// Every char value is then a valid Symbol, even one without a name, so any
// char may be static_cast to a Symbol. Nothing converts back implicitly.
enum class Symbol : char {
    add = '+',
    subtract = '-',
    multiply = '*',
    divide = '/',
    remainder = '%',
};

// underlying_type_t is an alias template, like make_unsigned_t.
static_assert(std::is_same_v<std::underlying_type_t<Symbol>, char>);

// --- The operation table -----------------------------------------------------

struct Operation {
    Symbol symbol;
    std::string_view name;
    BinaryOp apply;
};

// Captureless lambdas convert to plain function pointers, so each fits BinaryOp.
// CTAD deduces std::array<Operation, 5> from the initializers.
inline constexpr std::array operations{
    Operation{Symbol::add, "add", [](i32 a, i32 b) { return narrow(i64{a} + b); }},
    Operation{Symbol::subtract, "subtract", [](i32 a, i32 b) { return narrow(i64{a} - b); }},
    Operation{Symbol::multiply, "multiply", [](i32 a, i32 b) { return narrow(i64{a} * b); }},
    Operation{Symbol::divide, "divide", [](i32 a, i32 b) { return b == 0 ? Result{} : narrow(i64{a} / b); }},
    Operation{Symbol::remainder, "remainder", [](i32 a, i32 b) { return b == 0 ? Result{} : narrow(i64{a} % b); }},
};

// The projection &Operation::symbol makes find compare each element's symbol.
constexpr const Operation* find_operation(char symbol) {
    const auto found = std::ranges::find(operations, static_cast<Symbol>(symbol), &Operation::symbol);
    return found == operations.end() ? nullptr : &*found;
}
