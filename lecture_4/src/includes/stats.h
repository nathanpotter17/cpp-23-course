#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

// --- Choosing a type at compile time -----------------------------------------

// std::conditional_t picks the first type if the condition holds, else the
// second. Totals use the widest type of the same signedness as the values.
template <std::integral T>
using Wider = std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>;

// --- The summary -------------------------------------------------------------

template <std::integral T>
struct Summary {
    // Member aliases, named the way the standard containers name theirs.
    using value_type = T;
    using total_type = Wider<T>;

    std::size_t count = 0;
    value_type min{};
    value_type median{};
    value_type max{};
    total_type total{};
};

// Accepts any range of integers: a vector, an array, or a view over either.
template <std::ranges::input_range R>
    requires std::integral<std::ranges::range_value_t<R>>
auto summarize(R&& values) {
    // range_value_t is an alias template: the element type of R.
    using T = std::ranges::range_value_t<R>;

    // Our own sorted copy, whatever kind of range we were given.
    auto sorted = std::forward<R>(values) | std::ranges::to<std::vector>();
    std::ranges::sort(sorted);

    Summary<T> summary;
    if (sorted.empty()) {
        return summary;
    }

    summary.count = sorted.size();
    summary.min = sorted.front();
    summary.median = sorted[sorted.size() / 2];
    summary.max = sorted.back();
    summary.total = std::ranges::fold_left(sorted, Wider<T>{0}, std::plus{});
    return summary;
}
