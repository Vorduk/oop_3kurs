#include "TurnOnCommand.h"

TurnOnCommand::TurnOnCommand(std::shared_ptr<IDevice> device)
    : m_device(device) {
}

void TurnOnCommand::execute() {
    if (m_device) {
        std::cout << "[TurnOnCommand]: Executing for "
            << m_device->getType() << " (ID: " << m_device->getId() << ")" << std::endl;
        m_device->turnOn();
    }
}