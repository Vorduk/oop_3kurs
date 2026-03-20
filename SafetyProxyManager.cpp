#include "SafetyProxyManager.h"

SafetyProxyManager::SafetyProxyManager(std::shared_ptr<IClimateManager> realManager, std::shared_ptr<IIOManager> ioManager)
    : m_realManager(realManager)
    , m_ioManager(ioManager)
    , m_emergencyMode(false)
    , m_emergencyStrategy(nullptr) {
}

void SafetyProxyManager::setEmergencyStrategy(std::shared_ptr<IEmergencyStrategy> strategy)
{
    m_emergencyStrategy = strategy;
    std::cout << "[SafetyProxy]: Emergency strategy set to "
        << strategy->getName() << std::endl;
}

void SafetyProxyManager::setCriticalThresholds(double tempMin, double tempMax, double humidityMax)
{
    if (!m_emergencyStrategy) {
        m_emergencyStrategy = std::make_shared<TemperatureEmergencyStrategy>(
            tempMin, tempMax, humidityMax
        );
    }
    else if (auto tempStrategy = std::dynamic_pointer_cast<TemperatureEmergencyStrategy>(m_emergencyStrategy)) {
        tempStrategy->setThresholds(tempMin, tempMax, humidityMax);
    }
}

void SafetyProxyManager::setTargetParameters(const std::map<std::string, double>& targets)
{
    if (m_realManager) {
        m_realManager->setTargetParameters(targets);
    }
}

std::map<std::string, int> SafetyProxyManager::calculateCommands(const std::map<std::string, double>& currentReadings)
{

    // Проверка аварии делегируется стратегии
    if (m_emergencyStrategy && m_emergencyStrategy->isEmergency(currentReadings, m_emergencyReason)) {
        m_emergencyMode = true;
        std::cout << "\n[!!! EMERGENCY !!!]: " << m_emergencyReason << std::endl;
        return m_emergencyStrategy->getEmergencyCommands();
    }

    m_emergencyMode = false;

    if (m_realManager) {
        auto commands = m_realManager->calculateCommands(currentReadings);

        // Фильтрация опасных команд делегируется стратегии
        if (m_emergencyStrategy) {
            return m_emergencyStrategy->filterDangerousCommands(commands, currentReadings);
        }
        return commands;
    }

    return {};
}

bool SafetyProxyManager::isEmergencyMode() const
{
    return m_emergencyMode;
}

std::string SafetyProxyManager::getEmergencyReason() const
{
    return m_emergencyReason;
}