#include "EmergencyCoolingState.h"
#include "NormalState.h"
#include "ClimateManager.h"
#include <iostream>

EmergencyCoolingState::EmergencyCoolingState(double tempMax)
    : m_tempMax(tempMax) {
}

std::map<std::string, int> EmergencyCoolingState::handle(
    const std::map<std::string, double>& readings,
    const std::map<std::string, double>& targets,
    ClimateManager* context) {

    // Проверка, не спала ли температура до безопасного уровня
    std::map<std::string, double>::const_iterator tempIt = readings.find("temperature");
    if (tempIt != readings.end()) {
        double temp = tempIt->second;
        if (temp <= m_tempMax) {
            std::cout << "[EmergencyCoolingState] Temperature " << temp
                << " normalized, back to NormalState\n";
            context->setState(new NormalState(5.0, 40.0, 90.0));
            return context->computeStandardCommands(readings, targets);
        }
    }

    std::cout << "[EmergencyCoolingState]: Cooling: Conditioner=100, Ventilation=100\n";
    std::map<std::string, int> commands;
    commands["heater"] = 0;
    commands["conditioner"] = 100;
    commands["air_humidifier"] = 0;
    commands["ventilation"] = 100;
    commands["irrigation"] = 0;
    return commands;
}