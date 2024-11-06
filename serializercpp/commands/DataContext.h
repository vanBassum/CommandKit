#pragma once
#include "../streams/IStream.h"
#include "../serializers/Serializer.h"

class DataContext {
    IStream& stream;
    Serializer& serializer;

public:
    DataContext(IStream& s, Serializer& ser) : stream(s), serializer(ser) {}

    template<typename T>
    bool ReadData(T& data) {
        return serializer.Deserialize(stream, data);
    }

    template<typename T>
    bool WriteData(const T& data) {
        return serializer.Serialize(stream, data);
    }
};
