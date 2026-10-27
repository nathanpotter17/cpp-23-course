#pragma once

#include "includes/expression.h"
#include "includes/types.h"

#include <functional>
#include <optional>
#include <span>
#include <string_view>

// --- Callbacks ---------------------------------------------------------------

// Called once per input with what parse and evaluate made of it. A
// std::function holds any callable with this signature: a function, or a
// lambda that captures variables, which a plain function pointer can't hold.
using RowHandler = std::function<void(std::string_view input, const std::optional<Expression>& expression, Result value)>;

// --- Running a batch ---------------------------------------------------------

// Parses and evaluates every input, and leaves what to do with the results to
// the caller's handler.
inline void run_batch(std::span<const std::string_view> inputs, const RowHandler& handle) {
    for (const std::string_view input : inputs) {
        const std::optional<Expression> expression = parse(input);
        const Result value = expression ? expression->evaluate() : Result{};
        handle(input, expression, value);
    }
}
