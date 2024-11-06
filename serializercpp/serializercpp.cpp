#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <span>
#include "serializers/Serializer.h"
#include "serializers/ascii/AsciiVectorSerializer.h"
#include "streams/TerminalStream.h"
#include "commands/CommandContainer.h"
#include "commands/CommandHandler.h"
#include "commands/commands/PrintVectorCommand.h"


// Define constexpr instances of each serializer
constexpr AsciiVectorSerializer asciiVectorSerializer;
constexpr AsciiUint32Serializer asciiUint32Serializer;
constexpr AsciiResultSerializer asciiResultSerializer;

// Define arrays of serializers for use with std::span
constexpr const ITypeSerializerBase* asciiSerializers[] = {
    &asciiVectorSerializer,
    &asciiUint32Serializer,
    &asciiResultSerializer
};



// Test function
int main() {

    PrintVectorCommand printVectorCommand;



    CommandContainer container;
    container.AddCommand(0, &printVectorCommand);
    
    Serializer serializer(asciiSerializers, asciiSerializers);
    CommandHandler handler(container, serializer);

    TerminalStream stream;
    handler.ExecuteCommand(stream);

    return 0;
}
