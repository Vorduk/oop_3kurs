#pragma once
#include "ICommand.h"
#include "IDevice.h"
#include <memory>
#include <iostream>

// Команда установки мощности устройства с плавной регулировкой
class SetPowerCommand : public ICommand {
public:
    SetPowerCommand(std::shared_ptr<IAdjustableDevice> device, int powerLevel);
    void execute() override;
private:
    std::shared_ptr<IAdjustableDevice> m_device;
    int m_powerLevel;
};