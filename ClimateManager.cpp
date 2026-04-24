#include "ClimateManager.h"
#include <iostream>

ClimateManager::ClimateManager() {
    // Инициализация пустая — стратегии добавляются позже через addStrategy() или setRegulator()
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

std::map<std::string, double> ClimateManager::getTargets() const {
    return m_targets;
}

void ClimateManager::setRegulator(const std::string& parameter,
    std::shared_ptr<IRegulator> regulator) {

    // Поиск существующей стратегии для этого параметра
    for (auto& strategy : m_strategies) {
        if (strategy->getParamName() == parameter) {
            strategy->setRegulator(regulator);
            return;
        }
    }

    // Стратегии нет — создание новой
    if (parameter == "temperature") {
        // Температура управляется двумя устройствами:
        // - heater (нагреватель) — работает при положительном сигнале
        // - conditioner (кондиционер) — работает при отрицательном сигнале
        auto strategy = std::make_shared<ControlStrategy>(
            parameter,
            std::vector<std::string>{"heater", "conditioner"},
            regulator
        );
        m_strategies.push_back(strategy);
    }
    else if (parameter == "air_humidity") {
        // Влажность воздуха управляется увлажнителем
        auto strategy = std::make_shared<ControlStrategy>(
            parameter,
            std::vector<std::string>{"air_humidifier"},
            regulator
        );
        m_strategies.push_back(strategy);
    }
    else if (parameter == "soil_moisture") {
        // Влажность почвы управляется системой полива
        auto strategy = std::make_shared<ControlStrategy>(
            parameter,
            std::vector<std::string>{"irrigation"},
            regulator
        );
        m_strategies.push_back(strategy);
    }
    else {
        // Остальные параметры (если появятся) управляются одним устройством,
        // устройства будут определены позже через strategy->setDevices()
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

    // Обрабатывается каждая стратегия
    // ClimateManager не знает, как именно стратегия вычисляет команды —
    // это делегируется объекту ControlStrategy
    for (const auto& strategy : m_strategies) {
        // Стратегия возвращает map "устройство -> мощность" для своего параметра
        auto commands = strategy->calculate(current_readings, m_targets);

        // Объединение команды от всех стратегий
        for (const auto& [device, power] : commands) {
            auto it = all_commands.find(device);
            if (it != all_commands.end()) {
                // Конфликт: два параметра хотят управлять одним устройством
                // Выбирается максимальная мощность
                all_commands[device] = std::max(it->second, power);
            }
            else {
                all_commands[device] = power;
            }
        }
    }

    return all_commands;
}