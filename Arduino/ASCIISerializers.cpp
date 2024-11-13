#include "Serializer.h"
#include "IStream.h"
#include <Arduino.h>  // For converting values on Arduino (e.g., String)
#include "CommandStructures.h"


// Serialize an int to ASCII format with space separation and write to the stream
bool ASCII_Serialize(const Serializer& serializer, IStream& stream, const int& item, const Timeout& timeout) {
    String strData = String(item) + " ";  // Convert int to String with a space separator
    return stream.Write(strData.c_str(), strData.length(), timeout) == strData.length();
}

// Deserialize an ASCII-formatted int from the stream, stopping at a space or end of data
bool ASCII_Deserialize(const Serializer& serializer, IStream& stream, int& item, const Timeout& timeout) {
    char buffer[16];
    size_t index = 0;

    while (index < sizeof(buffer) - 1) {  // Reserve space for null-terminator
        char ch;
        size_t bytesRead = stream.Read(&ch, 1, timeout);  // Read one character at a time

        if (bytesRead == 0) {
            break;  // No more characters available
        }

        if (ch == ' ') {
            break;  // Stop reading at the space character
        }

        buffer[index++] = ch;  // Add character to buffer
    }

    buffer[index] = '\0';  // Null-terminate the buffer
    item = atoi(buffer);   // Convert ASCII to int
    return index > 0;      // Return true if any data was read
}


bool ASCII_Serialize(const Serializer& serializer, IStream& stream, const CommandRequest& item, const Timeout& timeout) {
    return serializer.Serialize(stream, (int)item.cmd, timeout);
}

bool ASCII_Deserialize(const Serializer& serializer, IStream& stream, CommandRequest& item, const Timeout& timeout) {
    int val;
    bool success = serializer.Deserialize(stream, val, timeout);
    item.cmd = (uint32_t)val;
    return success;
}

bool ASCII_Serialize(const Serializer& serializer, IStream& stream, const CommandResult& item, const Timeout& timeout) {
    return serializer.Serialize(stream, (int)item.resultCode, timeout);
}

bool ASCII_Deserialize(const Serializer& serializer, IStream& stream, CommandResult& item, const Timeout& timeout) {
    int val;
    bool success = serializer.Deserialize(stream, val, timeout);
    item.resultCode = (CommandResultCodes)val;
    return success;
}





// Define the array of SerializeItem for JSON serialization
static const SerializeItem asciiSerializers[] = {
    SERIALIZER_ENTRY(int,               ASCII_Serialize, ASCII_Deserialize),
    SERIALIZER_ENTRY(CommandRequest,    ASCII_Serialize, ASCII_Deserialize),
    SERIALIZER_ENTRY(CommandResult,     ASCII_Serialize, ASCII_Deserialize),
    // Add more types here using the same pattern
};

// Static method to create the JSON serializer
Serializer SerializerFactory::CreateAsciiSerializer() {
    return Serializer(asciiSerializers, sizeof(asciiSerializers) / sizeof(asciiSerializers[0]));
}
