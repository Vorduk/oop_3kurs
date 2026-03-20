#include "ClimateManager.h"
#include <iostream>
#include <cmath>

ClimateManager::ClimateManager()
    : m_temp_hysteresis(0.5)
    , m_humidity_hysteresis(2.0)
    , m_soil_hysteresis(3.0) {

    m_mapping["temperature"] = "heater";
    m_mapping["temperature_cool"] = "conditioner";
    m_mapping["air_humidity"] = "air_humidifier";
    m_mapping["soil_moisture"] = "irrigation";

    // По умолчанию используем On/Off регуляторы с гистерезисом
    // Они будут создаваться при необходимости
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

void ClimateManager::setRegulator(const std::string& parameter, std::shared_ptr<IRegulator> regulator) {
    m_regulators[parameter] = regulator;
    std::cout << "[ClimateManager]: Regulator set for " << parameter
        << " -> " << regulator->getName() << std::endl;
}

void ClimateManager::setHysteresis(double temp_hysteresis, double humidity_hysteresis, double soil_hysteresis) {
    m_temp_hysteresis = temp_hysteresis;
    m_humidity_hysteresis = humidity_hysteresis;
    m_soil_hysteresis = soil_hysteresis;
}

double ClimateManager::calculateControlSignal(const std::string& parameter, double current, double target) {
    // Паттерн Delegation: если есть регулятор для параметра, делегируем ему расчет
    auto it = m_regulators.find(parameter);
    if (it != m_regulators.end() && it->second) {
        return it->second->calculate(current, target);
    }

    // Иначе используем простой On/Off с гистерезисом
    double hysteresis = 0.0;
    if (parameter == "temperature") hysteresis = m_temp_hysteresis;
    else if (parameter == "air_humidity") hysteresis = m_humidity_hysteresis;
    else if (parameter == "soil_moisture") hysteresis = m_soil_hysteresis;

    if (std::abs(current - target) <= hysteresis) {
        return 0.0;
    }
    else if (current < target - hysteresis) {
        return std::min(100.0, (target - current) * 10.0);
    }
    else {
        return std::min(100.0, (current - target) * 10.0);
    }
}

std::map<std::string, int> ClimateManager::calculateCommands(
    const std::map<std::string, double>& current_readings) {

    std::map<std::string, int> commands;

    // Обработка температуры (нагрев)
    auto temp_it = current_readings.find("temperature");
    auto target_temp_it = m_targets.find("temperature");

    if (temp_it != current_readings.end() && target_temp_it != m_targets.end()) {
        double current = temp_it->second;
        double target = target_temp_it->second;

        double signal = calculateControlSignal("temperature", current, target);

        if (signal > 0) {
            // Холодно - включаем нагреватель
            commands["heater"] = static_cast<int>(signal);
            commands["conditioner"] = 0;
            std::cout << "[ClimateManager]: Temperature " << current << " C < " << target
                << " C, heater ON at " << static_cast<int>(signal) << "%" << std::endl;
        }
        else if (signal < 0) {
            // Жарко - включаем кондиционер
            commands["heater"] = 0;
            commands["conditioner"] = static_cast<int>(-signal);
            std::cout << "[ClimateManager]: Temperature " << current << " C > " << target
                << " C, conditioner ON at " << static_cast<int>(-signal) << "%" << std::endl;
        }
        else {
            commands["heater"] = 0;
            commands["conditioner"] = 0;
            std::cout << "[ClimateManager]: Temperature is optimal (" << current << " C)" << std::endl;
        }
    }

    // Обработка влажности воздуха (делегирование)
    auto hum_it = current_readings.find("air_humidity");
    auto target_hum_it = m_targets.find("air_humidity");

    if (hum_it != current_readings.end() && target_hum_it != m_targets.end()) {
        double current = hum_it->second;
        double target = target_hum_it->second;

        double signal = calculateControlSignal("air_humidity", current, target);

        if (signal > 0) {
            commands["air_humidifier"] = static_cast<int>(signal);
            std::cout << "[ClimateManager]: Humidity " << current << "% < " << target
                << "%, humidifier ON at " << static_cast<int>(signal) << "%" << std::endl;
        }
        else {
            commands["air_humidifier"] = 0;
        }
    }

    // Обработка влажности почвы (делегирование)
    auto soil_it = current_readings.find("soil_moisture");
    auto target_soil_it = m_targets.find("soil_moisture");

    if (soil_it != current_readings.end() && target_soil_it != m_targets.end()) {
        double current = soil_it->second;
        double target = target_soil_it->second;

        double signal = calculateControlSignal("soil_moisture", current, target);

        if (signal > 0) {
            commands["irrigation"] = static_cast<int>(signal);
            std::cout << "[ClimateManager]: Soil moisture " << current << "% < " << target
                << "%, irrigation ON at " << static_cast<int>(signal) << "%" << std::endl;
        }
        else {
            commands["irrigation"] = 0;
        }
    }

    return commands;
}