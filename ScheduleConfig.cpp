#include "ScheduleConfig.h"

ExtendedConfigManager::ExtendedConfigManager() {
    m_targets["temperature"] = 22.0;
    m_targets["air_humidity"] = 65.0;
    m_targets["soil_moisture"] = 45.0;
}

void ExtendedConfigManager::setSchedule(const std::string& deviceType,
    int intervalSeconds,
    int powerLevel) {
    m_schedules[deviceType] = ScheduleConfig(intervalSeconds, powerLevel);
    std::cout << "[Config]: Schedule set for " << deviceType
        << " - every " << intervalSeconds << "s at " << powerLevel << "%" << std::endl;
}

void ExtendedConfigManager::removeSchedule(const std::string& deviceType) {
    m_schedules.erase(deviceType);
}

ScheduleConfig ExtendedConfigManager::getSchedule(const std::string& deviceType) const {
    auto it = m_schedules.find(deviceType);
    return (it != m_schedules.end()) ? it->second : ScheduleConfig();
}

bool ExtendedConfigManager::hasSchedule(const std::string& deviceType) const {
    return m_schedules.find(deviceType) != m_schedules.end();
}

const std::map<std::string, ScheduleConfig>& ExtendedConfigManager::getAllSchedules() const {
    return m_schedules;
}