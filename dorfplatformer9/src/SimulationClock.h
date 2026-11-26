#pragma once

#include <algorithm>
#include <cstdint>

class SimulationClock {
public:
    static constexpr std::uint64_t ticksPerSecond = 60;
    static constexpr std::uint64_t nanosecondsPerSecond = 1'000'000'000;
    static constexpr double tickSeconds = 1.0 / ticksPerSecond;

    unsigned advance(std::uint64_t elapsedNs) {
        // Bound catch-up after a debugger stop or app suspension. Ordinary
        // render frames contribute their full elapsed time, including stalls.
        constexpr std::uint64_t maxElapsedNs = nanosecondsPerSecond / 4;
        remainder += std::min(elapsedNs, maxElapsedNs) * ticksPerSecond;
        const auto ticks = static_cast<unsigned>(remainder / nanosecondsPerSecond);
        remainder %= nanosecondsPerSecond;
        return ticks;
    }

private:
    std::uint64_t remainder = 0;
};
