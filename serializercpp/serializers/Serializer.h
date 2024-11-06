#pragma once
#include <span>
#include "ITypeSerializer.h"

class Serializer {
public:
    Serializer(std::span<const ITypeSerializerBase* const> serializers,
        std::span<const ITypeSerializerBase* const> deserializers)
        : serializers_(serializers), deserializers_(deserializers) {}

    template<typename T>
    bool Serialize(IStream& stream, const T& item) const {
        for (const auto& serializer : serializers_) {
            if (auto typedSerializer = dynamic_cast<const ITypeSerializer<T>*>(serializer)) {
                return typedSerializer->Serialize(stream, *this, &item);
            }
        }
        std::cerr << "No serializer found for type.\n";
        return false;
    }

    template<typename T>
    bool Deserialize(IStream& stream, T& item) const {
        for (const auto& deserializer : deserializers_) {
            if (auto typedDeserializer = dynamic_cast<const ITypeSerializer<T>*>(deserializer)) {
                return typedDeserializer->Deserialize(stream, *this, &item);
            }
        }
        std::cerr << "No deserializer found for type.\n";
        return false;
    }

private:
    std::span<const ITypeSerializerBase* const> serializers_;
    std::span<const ITypeSerializerBase* const> deserializers_;
};

