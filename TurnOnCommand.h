#pragma once
#include "ICommand.h"
#include "IDevice.h"
#include <memory>
#include <iostream>

// Команда включения устройства
class TurnOnCommand : public ICommand {
public:
    explicit TurnOnCommand(std::shared_ptr<IDevice> device);
    void execute() override;
private:
    std::shared_ptr<IDevice> m_device;
};