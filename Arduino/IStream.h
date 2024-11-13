#pragma once
#include <cstdint>
#include <cstddef>
#include "Timeout.h"

/// @brief An abstract base class defining a generic data stream interface.
/// This interface provides methods for reading, writing, and flushing data
/// with timeout capabilities. Implementations may represent different types
/// of streams, such as serial, TCP, or other communication channels.
class IStream {
public:
    /// @brief Reads data from the stream into a specified buffer.
    /// This function will attempt to read up to `size` bytes within the specified timeout.
    /// @param data A pointer to the buffer where the read data will be stored.
    /// @param size The maximum number of bytes to read.
    /// @param timeout A Timeout object specifying the maximum time to wait for data.
    /// @return The number of bytes successfully read. If the timeout expires before any data
    ///         is available, this function may return 0.
    virtual size_t Read(void* data, size_t size, const Timeout& timeout) = 0;

    /// @brief Writes data to the stream from a specified buffer.
    /// This function will attempt to write up to `size` bytes within the specified timeout.
    /// @param data A pointer to the buffer containing the data to write.
    /// @param size The number of bytes to write.
    /// @param timeout A Timeout object specifying the maximum time to wait for the write operation to complete.
    /// @return The number of bytes successfully written. If the timeout expires before the operation completes,
    ///         this function may return fewer bytes than requested.
    virtual size_t Write(const void* data, size_t size, const Timeout& timeout) = 0;

    /// @brief Flushes any buffered data within the stream.
    /// This function ensures that all pending data is sent within the specified timeout.
    /// Implementations may choose to make this function a no-op if buffering is not applicable.
    /// @param timeout A Timeout object specifying the maximum time to wait for flushing to complete.
    virtual void Flush(const Timeout& timeout) = 0;
};
