#pragma once

#include <print>

// --- A class that reports its own lifetime -----------------------------------

// Every special member function prints a line, so the program's output shows
// exactly when objects are created, copied, moved and destroyed.
class Tracer {
public:
    // explicit: a char can't silently turn into a Tracer.
    explicit Tracer(char name) : name_{name} {
        std::println("    [{}] constructed", name_);
    }

    // Copy constructor: a new object with the same value as `other`.
    Tracer(const Tracer& other) : name_{other.name_} {
        std::println("    [{}] copied", name_);
    }

    // Move constructor: takes over `other`'s contents. `other` stays alive and
    // will still be destroyed, so we mark it as emptied.
    Tracer(Tracer&& other) noexcept : name_{other.name_} {
        other.name_ = '_';
        std::println("    [{}] moved", name_);
    }

    // We never assign one Tracer to another, so we forbid it.
    Tracer& operator=(const Tracer&) = delete;
    Tracer& operator=(Tracer&&) = delete;

    // Destructor: runs automatically when the object's lifetime ends.
    ~Tracer() {
        std::println("    [{}] destroyed", name_);
    }

    char name() const {
        return name_;
    }

private:
    char name_;
};
