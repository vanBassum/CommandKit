#pragma once
#include <unordered_map>
#include "ICommand.h"

class CommandContainer {
    std::unordered_map<uint32_t, ICommand*> commands;

public:
    // Adds a command with a specific ID
    void AddCommand(uint32_t commandID, ICommand* command) {
        commands[commandID] = command;
    }

    // Retrieves a command by command ID
    ICommand* GetCommand(uint32_t commandID) const {
        auto it = commands.find(commandID);
        return it != commands.end() ? it->second : nullptr;
    }
};
