#pragma once
#include "IRegulator.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

/**
 * @brief Инкапсулирует логику управления для одного параметра микроклимата
 *
 * Паттерн Strategy: каждая стратегия знает, как управлять одним параметром,
 * используя регулятор и список устройств.
 */
class ControlStrategy {
public:
    /**
     * @brief Конструктор стратегии
     * @param paramName Имя параметра (temperature, air_humidity и т.д.)
     * @param devices Список устройств для управления
     * @param regulator Регулятор (PID, On/Off и т.д.)
     */
    ControlStrategy(const std::string& paramName,
        const std::vector<std::string>& devices,
        std::shared_ptr<IRegulator> regulator)
        : m_paramName(paramName)
        , m_devices(devices)
        , m_regulator(regulator) {
    }

    /**
     * @brief Рассчитать команды для устройств
     * @param currentReadings Текущие показания всех датчиков
     * @param targets Целевые значения параметров
     * @return Словарь deviceType -> powerLevel
     */
    std::map<std::string, int> calculate(
        const std::map<std::string, double>& currentReadings,
        const std::map<std::string, double>& targets) const {

        std::map<std::string, int> commands;

        auto currentIt = currentReadings.find(m_paramName);
        auto targetIt = targets.find(m_paramName);

        if (currentIt == currentReadings.end() || targetIt == targets.end()) {
            return commands;
        }

        double current = currentIt->second;
        double target = targetIt->second;
        double signal = m_regulator->calculate(current, target);

        // Логика распределения сигнала между устройствами
        // Для температуры: положительный сигнал -> первое устройство (heater),
        // отрицательный -> второе устройство (conditioner)
        if (m_devices.size() == 2) {
            if (signal > 0) {
                commands[m_devices[0]] = static_cast<int>(signal);
                commands[m_devices[1]] = 0;
                logAction(current, target, signal, m_devices[0]);
            }
            else if (signal < 0) {
                commands[m_devices[0]] = 0;
                commands[m_devices[1]] = static_cast<int>(-signal);
                logAction(current, target, -signal, m_devices[1]);
            }
            else {
                commands[m_devices[0]] = 0;
                commands[m_devices[1]] = 0;
                std::cout << "[ClimateManager]: " << m_paramName
                    << " is optimal (" << current << ")" << std::endl;
            }
        }
        // Для остальных параметров: одно устройство
        else if (m_devices.size() == 1) {
            if (signal > 0) {
                commands[m_devices[0]] = static_cast<int>(signal);
                logAction(current, target, signal, m_devices[0]);
            }
            else {
                commands[m_devices[0]] = 0;
            }
        }

        return commands;
    }

    const std::string& getParamName() const { return m_paramName; }

private:
    void logAction(double current, double target, double signal,
        const std::string& device) const {
        std::cout << "[ClimateManager]: " << m_paramName << " " << current
            << " -> " << target << ", " << device
            << " ON at " << static_cast<int>(signal) << "%" << std::endl;
    }

    std::string m_paramName;
    std::vector<std::string> m_devices;
    std::shared_ptr<IRegulator> m_regulator;
};