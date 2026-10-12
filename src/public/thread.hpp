#pragma once

#include <chrono>
#include <thread>

namespace bt::types {
using thread = std::thread;
using join_thread = std::jthread;

namespace this_thread {
    inline thread::id get_id() noexcept {
        return std::this_thread::get_id();
    }

    inline void yield() noexcept {
        std::this_thread::yield();
    }

    void sleep_for(long long ms) {
        auto dur = std::chrono::milliseconds(ms);
        std::this_thread::sleep_for(dur);
    }
} // namespace this_thread
} // namespace bt::types