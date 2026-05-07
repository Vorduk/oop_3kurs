#pragma once
#include "ICommand.h"
#include "IDevice.h"
#include <iostream>
#include <memory>

// Команда выключения устройства
class TurnOffCommand : public ICommand {
public:
    explicit TurnOffCommand(std::shared_ptr<IDevice> device);
    void execute() override;
private:
    std::shared_ptr<IDevice> m_device;
};