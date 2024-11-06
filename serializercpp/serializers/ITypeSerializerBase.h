#pragma once
#include <typeinfo>
#include "../streams/IStream.h"

class Serializer;
class ITypeSerializerBase {
public:
    virtual ~ITypeSerializerBase() = default;
    virtual bool Serialize(IStream& stream, const Serializer& serializer, const void* item) const = 0;
    virtual bool Deserialize(IStream& stream, const Serializer& serializer, void* item) const = 0;
};

