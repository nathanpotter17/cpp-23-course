#pragma once

#include <array>
#include <span>
#include <string>
#include <string_view>

// --- The board ---------------------------------------------------------------

// A Game of Life board that wraps around: the cells on the right edge are
// neighbors of those on the left edge, and the bottom row of the top row.
struct Grid {
    static constexpr int width = 8;
    static constexpr int height = 8;

    // Row-major order: all of row 0, then all of row 1, and so on. Cell (x, y)
    // is at index y * width + x. The {} sets every cell to false.
    std::array<bool, width * height> cells{};

    // x and y wrap around, so (-1, 0) is the last cell of row 0.
    bool alive(int x, int y) const;
    void set(int x, int y, bool value);

    // A view of one row: no copy, just a pointer to its first cell and a size.
    std::span<const bool> row(int y) const;

    // = default asks the compiler to compare every member: here, all cells.
    bool operator==(const Grid&) const = default;
};

// --- Functions on boards -----------------------------------------------------

// Counts the true values in any contiguous run of bools.
int count_alive(std::span<const bool> cells);

// Copies a pattern onto the grid with its top-left corner at (x, y).
// Each row of the pattern is text: '#' is alive, anything else is dead.
void place(Grid& grid, std::span<const std::string_view> pattern, int x, int y);

// The next generation: a live cell with 2 or 3 live neighbors survives,
// a dead cell with exactly 3 comes alive, and every other cell is dead.
Grid step(const Grid& grid);

// The grid as text, one line per row.
std::string render(const Grid& grid);
