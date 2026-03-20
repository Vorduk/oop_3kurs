#pragma once
#include "ConfigManager.h"
#include <chrono>
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

class ExtendedConfigManager : public ConfigManager {
public:
    ExtendedConfigManager();

    // Методы для работы с расписаниями
    void setSchedule(const std::string& deviceType, int intervalSeconds, int powerLevel);
    void removeSchedule(const std::string& deviceType);
    ScheduleConfig getSchedule(const std::string& deviceType) const;
    bool hasSchedule(const std::string& deviceType) const;
    const std::map<std::string, ScheduleConfig>& getAllSchedules() const;

private:
    std::map<std::string, ScheduleConfig> m_schedules;
};