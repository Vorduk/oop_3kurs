#include "ClimateManager.h"
#include <iostream>

ClimateManager::ClimateManager()
    : m_temp_hysteresis(1.0)
    , m_humidity_hysteresis(3.0)
    , m_soil_hysteresis(5.0) {

    // Маппинг по умолчанию
    m_mapping["temperature"] = "heater";      // температура -> нагреватель
    m_mapping["air_humidity"] = "air_humidifier";  // влажность -> увлажнитель
    m_mapping["soil_moisture"] = "irrigation";     // влажность почвы -> полив
}

void ClimateManager::setTargetParameters(const std::map<std::string, double>& targets) {
    m_targets = targets;
    std::cout << "[ClimateManager]: Targets set:" << std::endl;
    for (const auto& [param, value] : targets) {
        std::cout << "  " << param << " = " << value << std::endl;
    }
}

void ClimateManager::setMapping(const std::string& parameter, const std::string& device_type) {
    m_mapping[parameter] = device_type;
}

void ClimateManager::setHysteresis(double temp_hysteresis, double humidity_hysteresis, double soil_hysteresis) {
    m_temp_hysteresis = temp_hysteresis;
    m_humidity_hysteresis = humidity_hysteresis;
    m_soil_hysteresis = soil_hysteresis;
}

std::map<std::string, int> ClimateManager::calculateCommands(
    const std::map<std::string, double>& current_readings) {

    std::map<std::string, int> commands;

    // Обрабатка температуры
    auto temp_it = current_readings.find("temperature");
    auto target_temp_it = m_targets.find("temperature");
    if (temp_it != current_readings.end() && target_temp_it != m_targets.end()) {
        double current = temp_it->second;
        double target = target_temp_it->second;
        std::string deviceType = m_mapping["temperature"];

        if (current < target - m_temp_hysteresis) {
            commands[deviceType] = 100;
            std::cout << "[ClimateManager]: Temperature too low (" << current
                << " < " << target << "), turning ON heater" << std::endl;
        }
        else if (current > target + m_temp_hysteresis) {
            commands[deviceType] = 0;
            std::cout << "[ClimateManager]: Temperature too high (" << current
                << " > " << target << "), turning OFF heater" << std::endl;
        }
        else {
            commands[deviceType] = 0;
        }
    }

    // Обработка влажности воздуха
    auto humIt = current_readings.find("air_humidity");
    auto targetHumIt = m_targets.find("air_humidity");
    if (humIt != current_readings.end() && targetHumIt != m_targets.end()) {
        double current = humIt->second;
        double target = targetHumIt->second;
        std::string deviceType = m_mapping["air_humidity"];

        if (current < target - m_humidity_hysteresis) {
            commands[deviceType] = 100;  // Вкл увлажнитель
            std::cout << "[ClimateManager]: Humidity too low (" << current
                << " < " << target << "), turning ON humidifier" << std::endl;
        }
        else if (current > target + m_humidity_hysteresis) {
            commands[deviceType] = 0;    // Выкл увлажнитель
            std::cout << "[ClimateManager]: Humidity too high (" << current
                << " > " << target << "), turning OFF humidifier" << std::endl;
        }
        else {
            commands[deviceType] = 0;
        }
    }

    // Обработка влажности почвы
    auto soilIt = current_readings.find("soil_moisture");
    auto targetSoilIt = m_targets.find("soil_moisture");
    if (soilIt != current_readings.end() && targetSoilIt != m_targets.end()) {
        double current = soilIt->second;
        double target = targetSoilIt->second;
        std::string deviceType = m_mapping["soil_moisture"];

        if (current < target - m_soil_hysteresis) {
            commands[deviceType] = 100;  // Вкл полив
            std::cout << "[ClimateManager]: Soil moisture too low (" << current
                << " < " << target << "), turning ON irrigation" << std::endl;
        }
        else if (current > target + m_soil_hysteresis) {
            commands[deviceType] = 0;    // Выкл полив
            std::cout << "[ClimateManager]: Soil moisture too high (" << current
                << " > " << target << "), turning OFF irrigation" << std::endl;
        }
        else {
            commands[deviceType] = 0;
        }
    }

    return commands;
}
