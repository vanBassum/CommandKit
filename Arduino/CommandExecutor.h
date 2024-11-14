#pragma once
#include "CommandList.h"
#include "Framing.h"

/// @brief A class responsible for executing commands using a provided stream, command list, and factories for framing and serialization.
class CommandExecutor {
    IStream& baseStream;                ///< Reference to the base `Stream` used for communication.
    CommandList& commandList;          ///< Reference to the `CommandList` for command lookup.
    Serializer& serializer;

public:
    /// @brief Constructs a `CommandExecutor` with specified stream, command list, and serializer.
    /// @param stream The base stream used for communication.
    /// @param cmdList The command list for looking up and dispatching commands.
    /// @param serializer The serializer for serializing command data.
    CommandExecutor(IStream& stream, CommandList& cmdList, Serializer& serializer)
        : baseStream(stream), commandList(cmdList), serializer(serializer) {}

    /// @brief Main loop function that configures framing and serialization and then executes a command.
    /// This method configures the framing and serializer and attempts to read and execute a command from the stream within the specified timeout.
    /// @param framing A reference to a `Framing` instance to be configured.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the command operation.
    void Tick(Framing& framing, const Timeout& timeout) {
        ObjectStream objStream(framing, serializer);
        ExecuteCommand(objStream, timeout);
        framing.Flush(timeout);
    }

private:
    /// @brief Reads a command request from the stream, looks it up in the command list, and executes it.
    /// This function first reads a command request, looks up the associated command function in the command list, and then executes it.
    /// @param objStream An `ObjectStream` used for reading and writing serialized data.
    /// @param timeout A `Timeout` object specifying the maximum time allowed for the read and write operations.
    void ExecuteCommand(ObjectStream& objStream, const Timeout& timeout) {
        CommandRequest cmd;
        if (!objStream.Read(cmd, timeout)) { // Use timeout provided
            objStream.Write(CommandResult::Error(CommandResultCodes::SerializeError), Timeout::Milliseconds(100)); // Write with timeout
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
