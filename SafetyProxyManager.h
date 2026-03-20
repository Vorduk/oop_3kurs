#pragma once
#include "IClimateManager.h"
#include "IIOManager.h"
#include <memory>
#include <iostream>
#include <map>

// Паттерн Proxy: заместитель для IClimateManager с проверкой безопасности
class SafetyProxyManager : public IClimateManager {
private:
    std::shared_ptr<IClimateManager> m_realManager;
    IIOManager* m_ioManager;

    // Критические пороги
    double m_critical_temp_min = 0.0;
    double m_critical_temp_max = 45.0;
    double m_critical_humidity_max = 95.0;

    bool m_emergencyMode;
    std::string m_emergencyReason;

public:
    SafetyProxyManager(std::shared_ptr<IClimateManager> realManager, IIOManager* ioManager)
        : m_realManager(realManager), m_ioManager(ioManager), m_emergencyMode(false) {
    }

    void setTargetParameters(const std::map<std::string, double>& targets) override {
        // Делегируем реальному менеджеру
        if (m_realManager) {
            m_realManager->setTargetParameters(targets);
        }
    }

    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) override {

        // Проверка на аварийные ситуации
        if (checkEmergency(currentReadings)) {
            m_emergencyMode = true;
            std::cout << "\n[!!! EMERGENCY !!!]: " << m_emergencyReason << std::endl;
            return getEmergencyCommands();
        }

        m_emergencyMode = false;

        // Паттерн Proxy: делегируем реальному менеджеру в штатном режиме
        if (m_realManager) {
            auto commands = m_realManager->calculateCommands(currentReadings);
            return filterDangerousCommands(commands, currentReadings);
        }

        return {};
    }

    bool isEmergencyMode() const { return m_emergencyMode; }
    std::string getEmergencyReason() const { return m_emergencyReason; }

    void setCriticalThresholds(double tempMin, double tempMax, double humidityMax) {
        m_critical_temp_min = tempMin;
        m_critical_temp_max = tempMax;
        m_critical_humidity_max = humidityMax;
    }

private:
    bool checkEmergency(const std::map<std::string, double>& readings) {
        auto tempIt = readings.find("temperature");
        if (tempIt != readings.end()) {
            if (tempIt->second < m_critical_temp_min) {
                m_emergencyReason = "Temperature too LOW: " + std::to_string(tempIt->second) + " C";
                return true;
            }
            if (tempIt->second > m_critical_temp_max) {
                m_emergencyReason = "Temperature too HIGH: " + std::to_string(tempIt->second) + " C";
                return true;
            }
        }

        auto humIt = readings.find("air_humidity");
        if (humIt != readings.end() && humIt->second > m_critical_humidity_max) {
            m_emergencyReason = "Humidity too HIGH: " + std::to_string(humIt->second) + " %";
            return true;
        }

        return false;
    }

    std::map<std::string, int> getEmergencyCommands() {
        std::map<std::string, int> emergency;
        emergency["heater"] = 0;           // Отключить нагреватель
        emergency["conditioner"] = 100;    // Включить кондиционер на полную
        emergency["air_humidifier"] = 0;   // Отключить увлажнитель
        emergency["ventilation"] = 100;    // Включить вентиляцию
        emergency["irrigation"] = 0;       // Отключить полив
        return emergency;
    }

    std::map<std::string, int> filterDangerousCommands(
        const std::map<std::string, int>& commands,
        const std::map<std::string, double>& readings) {

        auto filtered = commands;

        auto tempIt = readings.find("temperature");
        if (tempIt != readings.end()) {
            // Если уже жарко, не включаем нагреватель
            if (tempIt->second > 35.0 && filtered["heater"] > 0) {
                filtered["heater"] = 0;
                std::cout << "[SafetyProxy]: Blocked heater - temperature too high" << std::endl;
            }
            // Если уже холодно, не включаем кондиционер
            if (tempIt->second < 15.0 && filtered["conditioner"] > 0) {
                filtered["conditioner"] = 0;
                std::cout << "[SafetyProxy]: Blocked conditioner - temperature too low" << std::endl;
            }
        }

        return filtered;
    }
};