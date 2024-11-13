#pragma once
#include "Framing.h"

/// @brief A `Framing` implementation that uses newline characters (`\n`) to frame messages.
/// The `NewLineFraming` class reads and writes data, treating each line ending with `\n`
/// as a separate message, and includes timeout support for these operations.
class NewLineFraming : public Framing {
    bool anyWritten = false;
public:
    /// @brief Constructs a `NewLineFraming` object using an existing `Stream`.
    /// @param stream The base stream to which newline framing operations will be applied.
    NewLineFraming(IStream& stream) : Framing(stream) {}

    /// @brief Reads data from the base stream until a newline character (`\n`) is encountered or the timeout expires.
    /// The newline character is discarded and not included in the output buffer.
    /// @param data A pointer to the buffer where the read data will be stored.
    /// @param size The maximum number of bytes to read (excluding the newline character).
    /// @param timeout A `Timeout` object specifying the maximum time allowed for reading.
    /// @return The number of bytes successfully read. Returns 0 if no data is available before the timeout.
    virtual size_t Read(void* data, size_t size, const Timeout& timeout) override {
        uint8_t* byteData = static_cast<uint8_t*>(data);
        size_t bytesRead = 0;

        // Read bytes until we encounter a newline or reach the buffer limit
        while (!timeout.Expired() && bytesRead < size) {
            if (baseStream.Read(byteData + bytesRead, 1, timeout)) {
                if (byteData[bytesRead] == '\n') {
                    return bytesRead; // Newline detected, end the frame
                }
                bytesRead++;
            }
        }

        return bytesRead; // Return the number of bytes read if no newline was found
    }

    /// @brief Writes data to the base stream and appends a newline character (`\n`) to signify the end of the frame.
    /// This function attempts to write the provided data, followed by a newline, within the specified timeout.
    /// @param data A pointer to the buffer containing the data to write.
    /// @param size The number of bytes to write (excluding the newline character).
    /// @param timeout A `Timeout` object specifying the maximum time allowed for writing.
    /// @return The total number of bytes written, including the newline character. Returns fewer bytes if the timeout expires.
    virtual size_t Write(const void* data, size_t size, const Timeout& timeout) override {
        size_t bytesWritten = baseStream.Write(data, size, timeout);

        if(bytesWritten)
            anyWritten = true;

        if (bytesWritten < size) {
            return bytesWritten; // Timeout or partial write, return immediately
        }

        return bytesWritten;
    }

    /// @brief Flushes any buffered data within the base stream, ensuring that all pending data is transmitted.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for flushing.
    virtual void Flush(const Timeout& timeout) override {
        if(anyWritten)
        {
            baseStream.Write("\n", 2, timeout);
        }
        baseStream.Flush(timeout); // Delegate flush to the base stream
    }
};
