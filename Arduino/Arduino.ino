#include "CommandList.h"
#include "SerialStream.h"
#include "CommandExecutor.h"
#include "NewLineFraming.h"
#include "Serializer.h"

// Sample Command Function
CommandResultCodes TestCommand(ObjectStream &objStream)
{
    Serial.write("Testing123\n");
    // Perform some command actions
    return Ok;
}

// Command lookup table
constexpr CommandLookupItem command_lookup[] = {
    {0, TestCommand},
    // Add more commands here as needed
};





SerialStream stream;
Serializer serializer = SerializerFactory::CreateAsciiSerializer();
StaticCommandList commandList(command_lookup, sizeof(command_lookup) / sizeof(command_lookup[0]));

FramingFactory framingFactory = [](IStream &stream, std::function<void(Framing &)> callback) {
    NewLineFraming framing(stream);
    callback(framing);
};

CommandExecutor executor(stream, commandList, framingFactory, serializer);

void setup()
{
    // Start the Serial communication at a baud rate of 9600
    Serial.begin(115200);

    // Wait for Serial connection (helpful if using Serial Monitor)
    while (!Serial);
}

void loop()
{
    executor.Tick(Timeout::Milliseconds(1000));
}
