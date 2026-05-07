// SetPowerCommand.cpp
#include "SetPowerCommand.h"
#include <iostream>

SetPowerCommand::SetPowerCommand(std::shared_ptr<IAdjustableDevice> device, int powerLevel)
    : m_device(device), m_powerLevel(powerLevel) {
}

void SetPowerCommand::execute() {
    if (m_device) {
        std::cout << "[SetPowerCommand] Executing to " << m_powerLevel << "%" << std::endl;
        m_device->setPower(m_powerLevel);
    }
}