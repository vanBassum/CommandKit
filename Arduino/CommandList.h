#pragma once
#include "ObjectStream.h"
#include "CommandStructures.h"
#include "Timeout.h"

/// @brief Type alias for a command function pointer.
/// A command function takes an `ObjectStream` reference as an argument
/// and returns a `CommandResultCodes` value indicating the result of the execution.
/// @param objStream Reference to an `ObjectStream` used for communication during command execution.
/// @return A `CommandResultCodes` value representing the command's success or failure.
using CommandFunc = CommandResultCodes (*)(ObjectStream& objStream);

/// @brief An interface for command lookup functionality in a command list.
/// This interface is designed to resolve command codes to their associated
/// functions, enabling dynamic dispatch of commands in a system.
class CommandList {
public:
    /// @brief Looks up a command function based on a command code.
    /// @param cmd The command code used to identify the function.
    /// @return A function pointer to the command handler associated with the command code.
    ///         Returns nullptr if the command is not found.
    virtual CommandFunc Lookup(uint32_t cmd) const = 0;
};

// CommandLookupItem structure for command lookup
struct CommandLookupItem {
    uint32_t cmd;
    CommandFunc execute;
};


/// @brief A concrete class implementing a static command list using a lookup table provided at construction.
/// This class performs a compile-time command lookup, allowing flexible initialization of command lists.
class StaticCommandList : public CommandList {
    const CommandLookupItem* commandTable;  ///< Pointer to the array of command lookup items.
    const size_t commandTableSize;          ///< The number of entries in the command lookup table.

public:
    /// @brief Constructs a `StaticCommandList` with a given command lookup table.
    /// @param table Pointer to a constant array of `CommandLookupItem`s representing the command list.
    /// @param size The number of items in the `commandTable`.
    StaticCommandList(const CommandLookupItem* table, size_t size) 
        : commandTable(table), commandTableSize(size) {}

    /// @brief Looks up a command function based on a command code.
    /// @param cmd The command code used to identify the function.
    /// @return A function pointer to the command handler associated with the command code.
    ///         Returns nullptr if the command is not found.
    CommandFunc Lookup(uint32_t cmd) const override {
        for (size_t i = 0; i < commandTableSize; ++i) {
            if (commandTable[i].cmd == cmd) {
                return commandTable[i].execute;
            }
        }
        return nullptr; // Command not found
    }
};