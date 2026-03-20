#include "ClimateManager.h"
#include <iostream>

ClimateManager::ClimateManager() {
}

void ClimateManager::setTargetParameters(const std::map<std::string, double>& targets) {
    m_targets = targets;
    std::cout << "[ClimateManager]: Targets set:" << std::endl;
    for (const auto& [param, value] : targets) {
        std::cout << "  " << param << " = " << value << std::endl;
    }
}

void ClimateManager::addStrategy(std::shared_ptr<ControlStrategy> strategy) {
    if (strategy) {
        m_strategies.push_back(strategy);
        std::cout << "[ClimateManager]: Added strategy for "
            << strategy->getParamName() << std::endl;
    }
}

void ClimateManager::setRegulator(const std::string& parameter,
    std::shared_ptr<IRegulator> regulator) {
    // Находим существующую стратегию или создаем новую
    for (auto& strategy : m_strategies) {
        if (strategy->getParamName() == parameter) {
            strategy->setRegulator(regulator);
            return;
        }
    }

    // Стратегии для температуры нужно создавать с двумя устройствами
    if (parameter == "temperature") {
        auto strategy = std::make_shared<ControlStrategy>(
            parameter,
            std::vector<std::string>{"heater", "conditioner"},
            regulator
        );
        m_strategies.push_back(strategy);
    }
    else {
        auto strategy = std::make_shared<ControlStrategy>(
            parameter,
            std::vector<std::string>{},
            regulator
        );
        m_strategies.push_back(strategy);
    }
}

std::map<std::string, int> ClimateManager::calculateCommands(
    const std::map<std::string, double>& current_readings) {

    std::map<std::string, int> all_commands;

    for (const auto& strategy : m_strategies) {
        auto commands = strategy->calculate(current_readings, m_targets);
        for (const auto& [device, power] : commands) {
            // Если устройство уже есть в командах, берем максимум
            auto it = all_commands.find(device);
            if (it != all_commands.end()) {
                all_commands[device] = std::max(it->second, power);
            }
            else {
                all_commands[device] = power;
            }
        }
    }

    return all_commands;
}