#include "EmergencyCoolingState.h"
#include "NormalState.h"
#include "ClimateManager.h"
#include <iostream>

EmergencyCoolingState::EmergencyCoolingState() {
}

std::map<std::string, int> EmergencyCoolingState::handle(
    const std::map<std::string, double>& readings,
    const std::map<std::string, double>& targets,
    ClimateManager* context) {

    // Проверка, не спала ли температура до безопасного уровня
    std::map<std::string, double>::const_iterator tempIt = readings.find("temperature");
    if (tempIt != readings.end()) {
        double temp = tempIt->second;
        std::map<std::string, double>::const_iterator targetIt = targets.find("temperature");
        double target = (targetIt != targets.end()) ? targetIt->second : 23.0;
        // Возврат в нормальный режим
        if (temp <= target + 2.0) {
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