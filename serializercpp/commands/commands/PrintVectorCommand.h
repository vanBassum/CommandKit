#pragma once
#include <iostream>
#include "../ICommand.h"
#include "../../models/Result.h"
#include "../../models/Vector.h"

class PrintVectorCommand : public ICommand {
public:
    bool Execute(DataContext& context) override {
        Vector pos, speed;

        // Read position and speed vectors from the context
        if (!context.ReadData(pos) || !context.ReadData(speed)) {
            std::cerr << "Failed to read input data.\n";
            return false;
        }

        // Print the vectors to the terminal
        std::cout << "Position Vector: (" << pos.x << ", " << pos.y << ")\n";
        std::cout << "Speed Vector: (" << speed.x << ", " << speed.y << ")\n";

        // Write the result back to indicate success
        Result result;
        result.success = true;
        return context.WriteData(result);
    }
};
