#include "TurnOffCommand.h"

TurnOffCommand::TurnOffCommand(std::shared_ptr<IDevice> device)
    : m_device(device) {
}

void TurnOffCommand::execute() {
    if (m_device) {
        std::cout << "[TurnOffCommand]: Executing for "
            << m_device->getType() << " (ID: " << m_device->getId() << ")" << std::endl;
        m_device->turnOff();
    }
}