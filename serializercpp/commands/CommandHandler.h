#include "CommandContainer.h"
#include "DataContext.h"

class CommandHandler {
    CommandContainer& commandContainer;
    Serializer& serializer;

public:
    CommandHandler(CommandContainer& container, Serializer& ser)
        : commandContainer(container), serializer(ser) {}

    bool ExecuteCommand(IStream& stream) {
        // Create a DataContext for the command execution
        DataContext context(stream, serializer);

        uint32_t commandId;
        if(!context.ReadData(commandId)) {
            std::cerr << "Error fetching command from stream\n";
            return false;
        }

        // Retrieve the command by its ID
        ICommand* cmd = commandContainer.GetCommand(commandId);
        if (!cmd) {
            std::cerr << "Unknown command ID: " << commandId << "\n";
            return false;
        }

        // Execute the command with the context
        return cmd->Execute(context);
    }
};
