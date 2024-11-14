#pragma once
#include "IStream.h"
#include "Timeout.h"

template <typename T>
int type_id()
{
    // The address of a unique static variable in each template instantiation serves as a unique ID.
    static const int id = []
    {
        static int counter = 0;
        return ++counter;
    }();
    return reinterpret_cast<int>(&id);
}

#define SERIALIZER_ENTRY(Type, SerializeFunc, DeserializeFunc)                               \
    {                                                                                        \
        type_id<Type>(),                                                                     \
        [](const Serializer &s, IStream &stream, const void *item, const Timeout &timeout) { \
            return SerializeFunc(s, stream, *static_cast<const Type *>(item), timeout);      \
        },                                                                                   \
        [](const Serializer &s, IStream &stream, void *item, const Timeout &timeout) {       \
            return DeserializeFunc(s, stream, *static_cast<Type *>(item), timeout);          \
        }}

class Serializer;
struct SerializeItem
{
    int id;
    bool (*serializer)(const Serializer &serializer, IStream &stream, const void *item, const Timeout &timeout);
    bool (*deserializer)(const Serializer &serializer, IStream &stream, void *item, const Timeout &timeout);
};

// Serializer class to handle both serialization and deserialization
class Serializer
{
    const SerializeItem *serializers;
    size_t count;

public:
    Serializer(const SerializeItem *serializers, size_t count)
        : serializers(serializers), count(count) {}

    template <typename T>
    bool Serialize(IStream &stream, const T &item, const Timeout &timeout) const
    {
        int id = type_id<T>();
        for (size_t i = 0; i < count; ++i)
        {
            const auto &entry = serializers[i];
            if (entry.id == id)
            {
                return entry.serializer(*this, stream, static_cast<const void *>(&item), timeout);
            }
        }
        printf("Type not supported for serialization\n");
        return false;
    }

    template <typename T>
    bool Deserialize(IStream &stream, T &item, const Timeout &timeout) const
    {
        int id = type_id<T>();
        for (size_t i = 0; i < count; ++i)
        {
            const auto &entry = serializers[i];
            if (entry.id == id)
            {
                return entry.deserializer(*this, stream, static_cast<void *>(&item), timeout);
            }
        }
        printf("Type not supported for deserialization\n");
        return false;
    }
};

// Factory class to create Serializer instances
class SerializerFactory
{
public:
    static Serializer CreateAsciiSerializer();
};
