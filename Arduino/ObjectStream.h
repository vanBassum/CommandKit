#pragma once
#include "Serializer.h"


/// @brief A class that combines a data `Stream` and `Serializer` to handle
/// reading and writing of objects over a stream with specified timeouts.
/// The `ObjectStream` class simplifies the process of serializing and
/// deserializing objects, making it easier to send and receive structured
/// data over a communication channel.
class ObjectStream {
    IStream& baseStream;    ///< Reference to the underlying `Stream` used for communication.
    Serializer& serializer; ///< Reference to the `Serializer` for object serialization and deserialization.

public:
    /// @brief Constructs an `ObjectStream` with the specified `Stream` and `Serializer`.
    /// @param stream The base stream used for communication.
    /// @param ser The serializer used for serializing and deserializing objects to and from the stream.
    ObjectStream(IStream& stream, Serializer& ser) : baseStream(stream), serializer(ser) {}

    /// @brief Reads an object of type `T` from the stream.
    /// This method uses the `Serializer` to deserialize an object from the `Stream`
    /// within the specified timeout.
    /// @tparam T The type of the object to read from the stream.
    /// @param item A reference to the object where the deserialized data will be stored.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the read operation.
    /// @return True if the object was successfully read and deserialized, false otherwise.
    template<typename T>
    bool Read(T& item, const Timeout& timeout) {
        return serializer.Deserialize(baseStream, item, timeout);
    }

    /// @brief Writes an object of type `T` to the stream.
    /// This method uses the `Serializer` to serialize an object and write it to the `Stream`
    /// within the specified timeout.
    /// @tparam T The type of the object to write to the stream.
    /// @param item The object to serialize and write to the stream.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the write operation.
    /// @return True if the object was successfully serialized and written, false otherwise.
    template<typename T>
    bool Write(const T& item, const Timeout& timeout) {
        return serializer.Serialize(baseStream, item, timeout);
    }
};
