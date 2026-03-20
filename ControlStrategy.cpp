#include "ControlStrategy.h"

/**
 * @brief Конструктор стратегии управления
 *
 * Инициализирует параметры стратегии. Регулятор может быть передан позже
 * через метод setRegulator(), если сейчас он недоступен.
 */
ControlStrategy::ControlStrategy(const std::string& paramName,
    const std::vector<std::string>& devices,
    std::shared_ptr<IRegulator> regulator)
    : m_paramName(paramName)
    , m_devices(devices)
    , m_regulator(regulator) {
    // Регулятор может быть передан позже через setRegulator()
    // Это позволяет конфигурировать стратегию после создания
}

/**
 * @brief Вычисление управляющих команд для одного параметра
 *
 * Вычисление сигнала передаётся регулятору.
 * Стратегия отвечает только за преобразование сигнала в команды
 * для конкретных устройств.
 */
std::map<std::string, int> ControlStrategy::calculate(
    const std::map<std::string, double>& currentReadings,
    const std::map<std::string, double>& targets) const {

    std::map<std::string, int> commands;

    // Поиск текущего и целевого значения параметра
    auto currentIt = currentReadings.find(m_paramName);
    auto targetIt = targets.find(m_paramName);

    // Если параметр не найден в одном из наборов, то нельзя управлять
    if (currentIt == currentReadings.end() || targetIt == targets.end()) {
        return commands;
    }

    // Проверка наличия регулятора
    if (!m_regulator) {
        return commands;  // Нет регулятора. Нет команд
    }

    // Делегирование расчёта сигнала регулятору
    double current = currentIt->second;
    double target = targetIt->second;
    double signal = m_regulator->calculate(current, target);

    // Преобразование сигнала в команды для устройств
    if (m_devices.empty()) {
        // Нет устройств для управления — только логирование
        logAction(current, target, signal, "no_device");
        return commands;
    }

    if (m_devices.size() == 2) {
        // Случай с двумя устройствами (температура: нагреватель + кондиционер)
        // Положительный сигнал -> вкл первое устройство (нагреватель)
        // Отрицательный сигнал -> вкл второе устройство (кондиционер)
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
            // Сигнал = 0 — оптимальное состояние, оба устройства выключены
            commands[m_devices[0]] = 0;
            commands[m_devices[1]] = 0;
            logAction(current, target, 0, "optimal");
        }
    }
    else if (m_devices.size() == 1) {
        // Случай с одним устройством (влажность воздуха, влажность почвы)
        // Положительный сигнал -> вкл устройство
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

/**
 * @brief Логирование действия для отладки и мониторинга
 *
 * Выводит информацию о текущем состоянии параметра и принятых решениях.
 */
void ControlStrategy::logAction(double current, double target, double signal,
    const std::string& device) const {

    if (device == "optimal") {
        // Оптимальное состояние — параметр в норме, устройства не нужны
        std::cout << "[ClimateManager]: " << m_paramName << " is optimal ("
            << current << ")" << std::endl;
    }
    else if (device == "no_device") {
        // Нет устройств для управления, только информация о рассогласовании
        std::cout << "[ClimateManager]: " << m_paramName << " " << current
            << " -> " << target << ", no device to control" << std::endl;
    }
    else {
        // Устройство включено с указанной мощностью
        std::cout << "[ClimateManager]: " << m_paramName << " " << current
            << " -> " << target << ", " << device
            << " ON at " << static_cast<int>(signal) << "%" << std::endl;
    }
}