#include "NormalState.h"
#include "ClimateManager.h"
#include "EmergencyCoolingState.h"
#include <iostream>

NormalState::NormalState(double temp_min, double temp_max, double humidity_max)
    : m_tempMin(temp_min), m_tempMax(temp_max), m_humidityMax(humidity_max) {
}

std::map<std::string, int> NormalState::handle(
    const std::map<std::string, double>& readings,
    const std::map<std::string, double>& targets,
    ClimateManager* context) {

    // Стандартные команды от регуляторов
    std::map<std::string, int> commands = context->computeStandardCommands(readings, targets);

    // Проверка аварийных ситуаций
    std::map<std::string, double>::const_iterator tempIt = readings.find("temperature");
    if (tempIt != readings.end()) {
        double temp = tempIt->second;

        if (temp < m_tempMin) {
            std::cout << "[NormalState] Temperature " << temp
                << " below emergency minimum " << m_tempMin
                << " - go to EmergencyHeatingState\n";
            commands.clear();
            commands["heater"] = 100;
            commands["conditioner"] = 0;
            commands["air_humidifier"] = 0;
            commands["ventilation"] = 0;
            commands["irrigation"] = 0;
            return commands;
        }

        if (temp > m_tempMax) {
            std::cout << "[NormalState] Temperature " << temp
                << " above emergency maximum " << m_tempMax
                << " – go to EmergencyCoolingState\n";
            context->setState(new EmergencyCoolingState()); // переключение на охлаждение
            commands.clear();
            commands["heater"] = 0;
            commands["conditioner"] = 100;
            commands["air_humidifier"] = 0;
            commands["ventilation"] = 100;
            commands["irrigation"] = 0;
            return commands;
        }
    }

    std::map<std::string, double>::const_iterator humIt = readings.find("air_humidity");
    if (humIt != readings.end()) {
        double hum = humIt->second;
        if (hum > m_humidityMax) {
            std::cout << "[NormalState] Humidity " << hum
                << " above emergency maximum " << m_humidityMax
                << " – ventilation\n";
            commands["air_humidifier"] = 0;
            commands["ventilation"] = 100;
        }
    }

    return commands;
}