#pragma once

/// @brief Enum representing possible result codes for command execution.
/// Each code indicates the success or failure status of a command, with specific
/// errors for serialization issues and command lookup failures.
enum CommandResultCodes {
    Ok = 0,              ///< Indicates successful execution of a command.
    GeneralError = 1,    ///< Indicates a general error occurred during command execution.
    SerializeError = 2,  ///< Indicates a failure occurred during the serialization or deserialization of data.
    CommandNotFound = 3, ///< Indicates that the specified command was not found in the command list.
};

/// @brief Structure representing a command request.
/// This structure contains information about the command to be executed.
struct CommandRequest {
    uint32_t cmd;  ///< Command identifier (code) representing the requested command.
};

/// @brief Structure representing the result of a command execution.
/// The `CommandResult` structure encapsulates the result code of a command
/// and provides static methods for generating common result types.
struct CommandResult {
    CommandResultCodes resultCode; ///< The result code of the command execution (OK or Error).

    /// @brief Creates a `CommandResult` representing a successful command execution.
    /// @return A `CommandResult` with the result code set to `CommandResultCodes::OK`.
    constexpr static CommandResult OK() { return CommandResult{CommandResultCodes::Ok}; }

    /// @brief Creates a `CommandResult` representing a failed command execution.
    /// @param code A `CommandResultCodes` value indicating the specific error.
    /// @return A `CommandResult` with the result code set to the specified error code.
    static CommandResult Error(CommandResultCodes code) { return CommandResult{code}; }
};
