#pragma once
#include "IClimateManager.h"
#include "IIOManager.h"
#include <memory>
#include <iostream>
#include <map>

/**
 * @brief Паттерн Proxy: заместитель для IClimateManager с проверкой безопасности
 */
class SafetyProxyManager : public IClimateManager {
public:
    SafetyProxyManager(std::shared_ptr<IClimateManager> realManager,
        std::shared_ptr<IIOManager> ioManager)
        : m_realManager(realManager), m_ioManager(ioManager), m_emergencyMode(false) {
    }

    void setTargetParameters(const std::map<std::string, double>& targets) override {
        if (m_realManager) {
            m_realManager->setTargetParameters(targets);
        }
    }

    std::map<std::string, int> calculateCommands(
        const std::map<std::string, double>& currentReadings) override {

        if (checkEmergency(currentReadings)) {
            m_emergencyMode = true;
            std::cout << "\n[!!! EMERGENCY !!!]: " << m_emergencyReason << std::endl;
            return getEmergencyCommands();
        }

        m_emergencyMode = false;

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
        emergency["heater"] = 0;
        emergency["conditioner"] = 100;
        emergency["air_humidifier"] = 0;
        emergency["ventilation"] = 100;
        emergency["irrigation"] = 0;
        return emergency;
    }

    std::map<std::string, int> filterDangerousCommands(
        const std::map<std::string, int>& commands,
        const std::map<std::string, double>& readings) {

        auto filtered = commands;

        auto tempIt = readings.find("temperature");
        if (tempIt != readings.end()) {
            if (tempIt->second > 35.0 && filtered["heater"] > 0) {
                filtered["heater"] = 0;
                std::cout << "[SafetyProxy]: Blocked heater - temperature too high" << std::endl;
            }
            if (tempIt->second < 15.0 && filtered["conditioner"] > 0) {
                filtered["conditioner"] = 0;
                std::cout << "[SafetyProxy]: Blocked conditioner - temperature too low" << std::endl;
            }
        }

        return filtered;
    }

    std::shared_ptr<IClimateManager> m_realManager;
    std::shared_ptr<IIOManager> m_ioManager;

    double m_critical_temp_min = 0.0;
    double m_critical_temp_max = 45.0;
    double m_critical_humidity_max = 95.0;

    bool m_emergencyMode;
    std::string m_emergencyReason;
};