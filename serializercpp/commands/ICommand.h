#pragma once
#include "DataContext.h"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual bool Execute(DataContext& context) = 0;
};

