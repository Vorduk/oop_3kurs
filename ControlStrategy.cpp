#include "ControlStrategy.h"

ControlStrategy::ControlStrategy(const std::string& paramName,
    const std::vector<std::string>& devices,
    std::shared_ptr<IRegulator> regulator)
    : m_paramName(paramName)
    , m_devices(devices)
    , m_regulator(regulator) {

    // Если нет регулятора, создаем OnOff по умолчанию
    if (!m_regulator) {
        // Будем создавать позже, когда понадобится
    }
}

std::map<std::string, int> ControlStrategy::calculate(
    const std::map<std::string, double>& currentReadings,
    const std::map<std::string, double>& targets) const {

    std::map<std::string, int> commands;

    auto currentIt = currentReadings.find(m_paramName);
    auto targetIt = targets.find(m_paramName);

    if (currentIt == currentReadings.end() || targetIt == targets.end()) {
        return commands;
    }

    if (!m_regulator) {
        return commands;
    }

    double current = currentIt->second;
    double target = targetIt->second;
    double signal = m_regulator->calculate(current, target);

    if (m_devices.empty()) {
        // Нет устройств для управления — просто логируем
        logAction(current, target, signal, "no_device");
        return commands;
    }

    if (m_devices.size() == 2) {
        // Два устройства: положительный сигнал -> первое, отрицательный -> второе
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
            logAction(current, target, 0, "optimal");
        }
    }
    else if (m_devices.size() == 1) {
        // Одно устройство
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

void ControlStrategy::logAction(double current, double target, double signal,
    const std::string& device) const {
    if (device == "optimal") {
        std::cout << "[ClimateManager]: " << m_paramName << " is optimal ("
            << current << ")" << std::endl;
    }
    else if (device == "no_device") {
        std::cout << "[ClimateManager]: " << m_paramName << " " << current
            << " -> " << target << ", no device to control" << std::endl;
    }
    else {
        std::cout << "[ClimateManager]: " << m_paramName << " " << current
            << " -> " << target << ", " << device
            << " ON at " << static_cast<int>(signal) << "%" << std::endl;
    }
}