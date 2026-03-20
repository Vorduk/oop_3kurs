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

/**
 * @brief Расширенный менеджер конфигурации с поддержкой таймеров
 *
 * Наследуется от ConfigManager и добавляет функциональность расписаний
 * для периодического включения устройств.
 */
class ExtendedConfigManager : public IConfigManager {
public:
    ExtendedConfigManager();

    double getTargetParameter(const std::string& parameter) override;
    void setTargetParameter(const std::string& parameter, double value) override;
    std::map<std::string, double> getAllTargets() override;

    // Методы для работы с расписаниями
    void setSchedule(const std::string& deviceType, int intervalSeconds, int powerLevel);
    void removeSchedule(const std::string& deviceType);
    ScheduleConfig getSchedule(const std::string& deviceType) const;
    bool hasSchedule(const std::string& deviceType) const;
    const std::map<std::string, ScheduleConfig>& getAllSchedules() const;

private:
    std::map<std::string, double> m_targets;
    std::map<std::string, ScheduleConfig> m_schedules;
};