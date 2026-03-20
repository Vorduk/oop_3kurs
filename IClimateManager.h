#pragma once
#include <map>
#include <string>

// Интерфейс менеджера климата
class IClimateManager {
public:
    virtual ~IClimateManager() = default;

    // Установка целевых параметров
    virtual void setTargetParameters(const std::map<std::string, double>& targets) = 0;

    // Рассчёт команд для устройств на основе текущих показаний
    // Возвращает: deviceType -> powerLevel (0-100)
    virtual std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) = 0;
};

