#include "ConfigManager.h"

ConfigManager::ConfigManager() {
    // Значения по умолчанию
    m_targets["temperature"] = 22.0;
    m_targets["air_humidity"] = 65.0;
    m_targets["soil_moisture"] = 45.0;
}

double ConfigManager::getTargetParameter(const std::string& parameter) {
    auto it = m_targets.find(parameter);
    if (it != m_targets.end()) {
        return it->second;
    }
    return 0.0;
}

void ConfigManager::setTargetParameter(const std::string& parameter, double value) {
    m_targets[parameter] = value;
}

std::map<std::string, double> ConfigManager::getAllTargets() {
    return m_targets;
}