#include "TotalEnergyExpert.h"
#include "Command.h"
#include "IIterator.h"
#include <iostream>

TotalEnergyExpert::TotalEnergyExpert(std::shared_ptr<IAggregate> history,
    const std::string& deviceType)
    : m_history(history), m_deviceType(deviceType) {
}

double TotalEnergyExpert::getValue() {
    if (!m_history) {
        std::cout << "[TotalEnergyExpert]: No history provided" << std::endl;
        return 0.0;
    }

    double total = 0.0;
    IIterator* it = m_history->createIterator();

    std::cout << "[TotalEnergyExpert]: Calculating total energy for "
        << m_deviceType << "..." << std::endl;

    while (it->hasNext()) {
        Command* cmd = static_cast<Command*>(it->next());
        if (cmd && cmd->getDeviceType() == m_deviceType) {
            total += cmd->getPower();
        }
    }

    delete it;

    std::cout << "[TotalEnergyExpert]: Total energy for " << m_deviceType
        << " = " << total << std::endl;

    return total;
}