#pragma once
#include <Arduino.h>
#include "IStream.h"
#include "Timeout.h"

/// @brief A `Stream` implementation using Arduino's Serial interface for UART communication.
/// The `SerialStream` class provides read, write, and flush functionality with timeout support,
/// utilizing Arduino's `Serial` object.
class SerialStream : public IStream {
public:
    /// @brief Reads data from the UART into a specified buffer.
    /// This method will attempt to read up to `size` bytes within the specified timeout period.
    /// @param data A pointer to the buffer where the read data will be stored.
    /// @param size The maximum number of bytes to read.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for reading.
    /// @return The number of bytes actually read. If the timeout expires before any data is available, this returns 0.
    virtual size_t Read(void* data, size_t size, const Timeout& timeout) override {
        uint8_t* byteData = static_cast<uint8_t*>(data);
        size_t bytesRead = 0;

        // Continue reading while we have time and data left to read
        while (!timeout.Expired() && bytesRead < size) {
            if (Serial.available() > 0) {
                bytesRead += Serial.readBytes(byteData + bytesRead, size - bytesRead);
            }
        }

        return bytesRead;
    }

    /// @brief Writes data to the UART from a specified buffer.
    /// This method will attempt to write up to `size` bytes within the specified timeout period.
    /// @param data A pointer to the buffer containing the data to write.
    /// @param size The number of bytes to write.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for writing.
    /// @return The number of bytes actually written. If the timeout expires, this may be fewer than requested.
    virtual size_t Write(const void* data, size_t size, const Timeout& timeout) override {
        const uint8_t* byteData = static_cast<const uint8_t*>(data);
        size_t bytesWritten = 0;

        // Write data within the specified timeout
        while (!timeout.Expired() && bytesWritten < size) {
            bytesWritten += Serial.write(byteData + bytesWritten, size - bytesWritten);
        }

        return bytesWritten;
    }

    /// @brief Flushes any buffered data to the UART.
    /// This function waits for all outgoing data to be transmitted within the specified timeout period.
    /// If the timeout expires before flushing is complete, it stops waiting.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for flushing.
    virtual void Flush(const Timeout& timeout) override {

        // Wait for Serial to finish transmitting or until timeout expires
        while (!timeout.Expired() && Serial.availableForWrite() < 64) {
            // Do nothing, just wait for Serial to finish transmitting
        }
        Serial.flush();
    }
};
