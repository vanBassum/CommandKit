#pragma once
#include <cstdint>
#include <functional>
#include "CommandList.h"
#include "Framing.h"

/// @brief Factory type for creating and configuring `Framing` instances.
/// This factory function takes a `Stream` reference and a callback function,
/// which is called with the created `Framing` instance.
using FramingFactory = std::function<void(IStream&, std::function<void(Framing&)>)>;

/// @brief A class responsible for executing commands using a provided stream, command list, and factories for framing and serialization.
/// The `CommandExecutor` class manages the flow of receiving a command request, looking up the command in a command list,
/// and executing the command with an optional timeout for each operation.
class CommandExecutor {
    IStream& baseStream;                ///< Reference to the base `Stream` used for communication.
    CommandList& commandList;          ///< Reference to the `CommandList` for command lookup.
    FramingFactory framingFactory;     ///< Factory function for creating `Framing` instances.
    Serializer& serializer;

public:
    /// @brief Constructs a `CommandExecutor` with specified stream, command list, and factories for framing and serialization.
    /// @param stream The base stream used for communication.
    /// @param cmdList The command list for looking up and dispatching commands.
    /// @param framingFac The factory function for configuring and creating `Framing` instances.
    /// @param serializerFac The factory function for configuring and creating `Serializer` instances.
    CommandExecutor(IStream& stream, CommandList& cmdList,
                    FramingFactory framingFac, Serializer& serializer)
        : baseStream(stream), commandList(cmdList),
          framingFactory(framingFac), serializer(serializer) {}

    /// @brief Main loop function that configures framing and serialization and then executes a command.
    /// This method configures the framing and serializer using the provided factories and attempts to read
    /// and execute a command from the stream within the specified timeout.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the command operation.
    void Tick(const Timeout& timeout) {
        // Configure framing and serializer through factories
        framingFactory(baseStream, [this, &timeout](Framing& framing) {
            ObjectStream objStream(framing, serializer);
            ExecuteCommand(objStream, timeout);
            framing.Flush(timeout);
        });
    }

private:
    /// @brief Reads a command request from the stream, looks it up in the command list, and executes it.
    /// This function first reads a command request, looks up the associated command function in the
    /// command list, and then executes it. If the command cannot be found or the read fails, it writes an error result.
    /// @param objStream An `ObjectStream` used for reading and writing serialized data.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the read and write operations.
    void ExecuteCommand(ObjectStream& objStream, const Timeout& timeout) {
        CommandRequest cmd;
        if (!objStream.Read(cmd, timeout)) { // Use timeout provided
            objStream.Write(CommandResult::Error(CommandResultCodes::SerializeError), timeout); // Write with timeout
            return;
        }

        // Lookup and execute the command
        CommandFunc commandFunc = commandList.Lookup(cmd.cmd);
        if (commandFunc) {
            CommandResultCodes result = commandFunc(objStream); // Execute command
            objStream.Write(CommandResult{result}, timeout);    // Write result back to stream
        } else {
            objStream.Write(CommandResult::Error(CommandResultCodes::CommandNotFound), timeout); // Error for unknown command
        }
    }
};
