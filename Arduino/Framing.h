#pragma once
#include <cstdint>
#include <cstddef>
#include "Timeout.h"
#include "IStream.h"


/// @brief A base class for data stream framing, extending the Stream interface.
/// The Framing class is used to add frame boundaries to the underlying stream,
/// such as newline or delimiter-based frames, and provides mechanisms for 
/// reading, writing, and flushing frames.
class Framing : public IStream {
protected:
    IStream& baseStream; ///< Reference to the underlying base stream.

public:
    /// @brief Constructs a Framing object over an existing Stream.
    /// @param stream The base stream to which framing operations will be applied.
    Framing(IStream& stream) : baseStream(stream) {}
};
