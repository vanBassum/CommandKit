#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../ITypeSerializer.h"
#include "../../models/Result.h"
#include "../../models/Vector.h"

class AsciiVectorSerializer : public ITypeSerializer<Vector> {
public:
    bool Serialize(IStream& stream, const Serializer& serializer, const Vector& item) const override {
        std::ostringstream oss;
        oss << "(" << std::fixed << std::setprecision(1) << item.x << "," << item.y << ")";

        std::string serializedData = oss.str();
        return stream.Write(serializedData.c_str(), serializedData.size()) > 0;
    }

    bool Deserialize(IStream& stream, const Serializer& serializer, Vector& item) const override {
        std::string data;
        char ch;
        bool record = false;

        // Read one character at a time until a space, newline, or end of stream
        while (true) {
            size_t bytesRead = stream.Read(&ch, 1, 500000); // 5-second timeout for each character read
            if (bytesRead == 0) {
                return false; // Timeout or no data available
            }

            // Stop reading if we encounter a space or newline character
            if (ch == '\n' || ch == '\r') {
                break;
            }
            else if (ch == '(') {
                record = true;

            }

            if(record)
                data += ch;

            if (ch == ')') {
                break;
            }
        }

        // Now we parse the data string in the format "(x,y)"
        if (data.front() == '(' && data.back() == ')') {
            data = data.substr(1, data.size() - 2); // Remove parentheses
            size_t commaPos = data.find(',');

            if (commaPos != std::string::npos) {
                try {
                    item.x = std::stof(data.substr(0, commaPos));
                    item.y = std::stof(data.substr(commaPos + 1));
                    return true;
                }
                catch (...) {
                    return false; // Conversion failed
                }
            }
        }
        return false;
    }
};



class AsciiUint32Serializer : public ITypeSerializer<uint32_t> {
public:
    bool Serialize(IStream& stream, const Serializer& serializer, const uint32_t& item) const override {
        std::ostringstream oss;
        oss << item;

        std::string serializedData = oss.str();
        return stream.Write(serializedData.c_str(), serializedData.size(), 0) > 0;
    }

    bool Deserialize(IStream& stream, const Serializer& serializer, uint32_t& item) const override {
        std::string data;
        char ch;

        // Read one byte at a time until a space, newline, or carriage return is found
        while (true) {
            size_t bytesRead = stream.Read(&ch, 1, 500000); // 5-second timeout example for each byte read
            if (bytesRead == 0) {
                return false; // No data read (timeout or end of stream)
            }

            // Check for terminating characters
            if (ch == ' ' || ch == '\n' || ch == '\r') {
                break;
            }

            // Append the character to the data string if it is valid
            data += ch;
        }

        // Attempt to convert the accumulated string to uint32_t
        try {
            item = static_cast<uint32_t>(std::stoul(data)); // Convert string to uint32_t
            return true;
        }
        catch (...) {
            return false; // Conversion failed
        }
    }
};

class AsciiResultSerializer : public ITypeSerializer<Result> {
public:
    bool Serialize(IStream& stream, const Serializer& serializer, const Result& item) const override {
        std::string serializedData = item.success ? "Success" : "Error";
        return stream.Write(serializedData.c_str(), serializedData.size(), 0) > 0;
    }

    bool Deserialize(IStream& stream, const Serializer& serializer, Result& item) const override {
        return false; // Not implemented
    }
};

