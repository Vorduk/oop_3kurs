#pragma once
#include "ICommand.h"
#include <memory>
#include <map>

// Инициатор – пульт управления
class RemoteControl {
public:
    void setCommand(int slot, std::shared_ptr<ICommand> command);
    void pressButton(int slot);
private:
    std::map<int, std::shared_ptr<ICommand>> m_commands;
};