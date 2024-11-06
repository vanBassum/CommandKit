#pragma once
#include "ITypeSerializerBase.h"

template<typename T>
class ITypeSerializer : public ITypeSerializerBase {
public:
    bool Serialize(IStream& stream, const Serializer& serializer, const void* item) const override {
        return Serialize(stream, serializer, *static_cast<const T*>(item));
    }

    bool Deserialize(IStream& stream, const Serializer& serializer, void* item) const override {
        return Deserialize(stream, serializer, *static_cast<T*>(item));
    }

    virtual bool Serialize(IStream& stream, const Serializer& serializer, const T& item) const = 0;
    virtual bool Deserialize(IStream& stream, const Serializer& serializer, T& item) const = 0;
};

