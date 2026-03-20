#include "ConfigManager.h"

/**
 * @brief Конструктор, устанавливает значения параметров по умолчанию
 *
 * Инициализирует три основных параметра микроклимата теплицы:
 * температура 22°C — комфортный диапазон для большинства растений
 * влажность воздуха 65% — оптимальная для фотосинтеза
 * влажность почвы 45% — достаточная для корневой системы
 */
ConfigManager::ConfigManager() {
    m_targets["temperature"] = 22.0;
    m_targets["air_humidity"] = 65.0;
    m_targets["soil_moisture"] = 45.0;
}

double ConfigManager::getTargetParameter(const std::string& parameter) {
    // Поиск параметра в хранилище
    auto it = m_targets.find(parameter);

    // Если параметр найден возвращается его значение
    // Если не найден возвращается 0.0
    return (it != m_targets.end()) ? it->second : 0.0;
}

void ConfigManager::setTargetParameter(const std::string& parameter, double value) {
    // Установка нового целевого значения для параметра
    // Если параметра не было — он создаётся автоматически
    m_targets[parameter] = value;
}

std::map<std::string, double> ConfigManager::getAllTargets() {
    // Возврат копии всех целевых параметров
    // Копия защищает внутренние данные от случайного изменения
    return m_targets;
}