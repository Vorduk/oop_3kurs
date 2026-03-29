#include "SafetyProxyManager.h"
#include <iostream>

SafetyProxyManager::SafetyProxyManager(std::shared_ptr<IClimateManager> realManager,
    std::shared_ptr<IIOManager> ioManager,
    std::shared_ptr<IEmergencyStrategy> strategy)
    : m_realManager(realManager)
    , m_ioManager(ioManager)
    , m_emergencyStrategy(strategy)
    , m_emergencyMode(false)
{
    if (!m_emergencyStrategy) {
        // Стратегия по умолчанию (безопасные пороги)
        m_emergencyStrategy = std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);
    }
    std::cout << "[SafetyProxyManager]: Proxy created with strategy "
        << m_emergencyStrategy->getName() << std::endl;
}

void SafetyProxyManager::setTargetParameters(const std::map<std::string, double>& targets) {
    if (m_realManager) {
        m_realManager->setTargetParameters(targets);
    }
}

std::map<std::string, int> SafetyProxyManager::calculateCommands(
    const std::map<std::string, double>& currentReadings)
{
    // Проверка аварии через стратегию
    std::string reason;
    if (m_emergencyStrategy && m_emergencyStrategy->isEmergency(currentReadings, reason)) {
        m_emergencyMode = true;
        m_emergencyReason = reason;
        logEmergency(reason);
        return m_emergencyStrategy->getEmergencyCommands();
    }

    m_emergencyMode = false;
    if (m_realManager) {
        auto commands = m_realManager->calculateCommands(currentReadings);
        // Фильтрация опасных команд через стратегию
        if (m_emergencyStrategy) {
            return m_emergencyStrategy->filterDangerousCommands(commands, currentReadings);
        }
        return commands;
    }
    return {};
}

bool SafetyProxyManager::isEmergencyMode() const {
    return m_emergencyMode;
}

std::string SafetyProxyManager::getEmergencyReason() const {
    return m_emergencyReason;
}

void SafetyProxyManager::logEmergency(const std::string& reason) {
    std::cout << "[SafetyProxyManager]: EMERGENCY! " << reason << std::endl;
}