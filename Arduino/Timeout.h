#pragma once
#include <Arduino.h>
#include <cstdint>

/// @brief A utility class for managing timeouts based on elapsed milliseconds.
/// This class is designed to handle unsigned integer overflow gracefully, 
/// allowing for reliable timeout behavior even on systems with 32-bit millisecond counters.
class Timeout {
    const uint64_t startTimeMs;   ///< The timestamp (in milliseconds) when the timeout was created.
    const uint64_t timeoutMs;     ///< The duration of the timeout (in milliseconds).

public:
    /// @brief Creates a Timeout object with a specified duration.
    /// @param ms The timeout duration in milliseconds.
    /// @return A Timeout object that starts at the current time and expires after the given duration.
    static Timeout Milliseconds(uint64_t ms) {
        return Timeout(CurrentMilliseconds(), ms);
    }

    /// @brief Constructs a Timeout object with a specific start time and duration.
    /// @param startMs The start time in milliseconds.
    /// @param durationMs The duration of the timeout in milliseconds.
    Timeout(uint64_t startMs, uint64_t durationMs) 
        : startTimeMs(startMs), timeoutMs(durationMs) {}

    /// @brief Checks if the timeout has expired.
    /// This method is safe for systems with 32-bit millisecond timers, as it accounts for overflow.
    /// @return True if the timeout has expired, false otherwise.
    bool Expired() const {
        return (CurrentMilliseconds() - startTimeMs) >= timeoutMs;
    }

    /// @brief Gets the remaining time until the timeout expires, in milliseconds.
    /// If the timeout has already expired, this method returns 0.
    /// @return The remaining time in milliseconds, or 0 if the timeout has expired.
    uint64_t RemainingMilliseconds() const {
        if (Expired()) {
            return 0;
        } else {
            return timeoutMs - (CurrentMilliseconds() - startTimeMs);
        }
    }

private:
    /// @brief Retrieves the current time in milliseconds.
    /// This function should be replaced with the platform-specific millisecond counter,
    /// such as Arduino's millis() function.
    /// @return The current time in milliseconds.
    static uint32_t CurrentMilliseconds() {
        return millis();  // Replace with platform-specific timing function.
    }
};
