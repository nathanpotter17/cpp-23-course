#pragma once

#include <cstddef>
#include <new>
#include <type_traits>

// --- A bump allocator --------------------------------------------------------

// An arena owns one block of memory and hands out pieces of it from front to
// back. Pieces are never freed one by one: the whole block is freed when the
// arena is destroyed. Game engines use arenas for short-lived data, such as
// everything one frame needs.
class Arena {
public:
    explicit Arena(std::size_t capacity);

    // The rule of five: a class that owns a raw resource writes (or deletes)
    // all five special member functions, because the generated ones would
    // just copy the pointer.
    Arena(const Arena&) = delete; // two owners would both delete[] the block
    Arena& operator=(const Arena&) = delete;
    Arena(Arena&& other) noexcept;
    Arena& operator=(Arena&& other) noexcept;
    ~Arena();

    // `size` bytes starting at a multiple of `alignment`, or nullptr if the
    // arena is full. `alignment` must be a power of two.
    void* allocate(std::size_t size, std::size_t alignment);

    // A member function template: the caller picks T, as in create<Vec4>().
    template <typename T>
    T* create() {
        // The arena never runs destructors, so T must not need one.
        static_assert(std::is_trivially_destructible_v<T>);

        void* memory = allocate(sizeof(T), alignof(T));
        if (memory == nullptr) {
            return nullptr;
        }

        // Placement new constructs a T in memory we already have.
        return new (memory) T{};
    }

    std::size_t used() const {
        return used_;
    }

    std::size_t capacity() const {
        return capacity_;
    }

    // Where a pointer from this arena lies, in bytes from the block's start.
    std::size_t offset_of(const void* pointer) const;

private:
    std::byte* memory_ = nullptr;
    std::size_t capacity_ = 0;
    std::size_t used_ = 0;
};
