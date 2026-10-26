#pragma once

#include <chrono>
#include <thread>

namespace bt::types {

using Thread = std::thread;
using JoinThread = std::jthread;

namespace this_thread {
    inline auto get_id() noexcept -> Thread::id {
        return std::this_thread::get_id();
    }

    inline void yield() noexcept {
        std::this_thread::yield();
    }

    void sleep_for(long long millis) {
        auto dur = std::chrono::milliseconds(millis);
        std::this_thread::sleep_for(dur);
    }
} // namespace this_thread

} // namespace bt::types