#pragma once
#include "IConfigManager.h"
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <iostream>

struct ScheduleConfig {
    std::chrono::seconds interval;
    int powerLevel;
    bool enabled;

    ScheduleConfig() : interval(0), powerLevel(0), enabled(false) {}
    ScheduleConfig(int seconds, int power)
        : interval(seconds), powerLevel(power), enabled(true) {
    }
};

class ExtendedConfigManager : public IConfigManager {
private:
    std::map<std::string, double> m_targets;
    std::map<std::string, ScheduleConfig> m_schedules;

public:
    ExtendedConfigManager() {
        m_targets["temperature"] = 22.0;
        m_targets["air_humidity"] = 65.0;
        m_targets["soil_moisture"] = 45.0;
    }

    double getTargetParameter(const std::string& parameter) override {
        auto it = m_targets.find(parameter);
        return (it != m_targets.end()) ? it->second : 0.0;
    }

    void setTargetParameter(const std::string& parameter, double value) override {
        m_targets[parameter] = value;
    }

    std::map<std::string, double> getAllTargets() override {
        return m_targets;
    }

    void setSchedule(const std::string& deviceType, int intervalSeconds, int powerLevel) {
        m_schedules[deviceType] = ScheduleConfig(intervalSeconds, powerLevel);
        std::cout << "[Config]: Schedule set for " << deviceType
            << " - every " << intervalSeconds << "s at " << powerLevel << "%" << std::endl;
    }

    void removeSchedule(const std::string& deviceType) {
        m_schedules.erase(deviceType);
    }

    ScheduleConfig getSchedule(const std::string& deviceType) const {
        auto it = m_schedules.find(deviceType);
        return (it != m_schedules.end()) ? it->second : ScheduleConfig();
    }

    bool hasSchedule(const std::string& deviceType) const {
        return m_schedules.find(deviceType) != m_schedules.end();
    }

    const std::map<std::string, ScheduleConfig>& getAllSchedules() const {
        return m_schedules;
    }
};