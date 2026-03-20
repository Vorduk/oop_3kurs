#include "TemperatureEmergencyStrategy.h"

TemperatureEmergencyStrategy::TemperatureEmergencyStrategy(double tempMin, double tempMax, double humidityMax)
    : m_tempMin(tempMin), m_tempMax(tempMax), m_humidityMax(humidityMax) {
}

std::map<std::string, int> TemperatureEmergencyStrategy::getEmergencyCommands() const
{
    return {
        {"heater", 0},           // Отключить нагреватель
        {"conditioner", 100},    // Включить кондиционер на полную
        {"air_humidifier", 0},   // Отключить увлажнитель
        {"ventilation", 100},    // Включить вентиляцию
        {"irrigation", 0}        // Отключить полив
    };
}

bool TemperatureEmergencyStrategy::isEmergency(const std::map<std::string, double>& readings, std::string& reason) const
{
    auto tempIt = readings.find("temperature");
    if (tempIt != readings.end()) {
        if (tempIt->second < m_tempMin) {
            reason = "Temperature too LOW: " + std::to_string(tempIt->second) + " C";
            return true;
        }
        if (tempIt->second > m_tempMax) {
            reason = "Temperature too HIGH: " + std::to_string(tempIt->second) + " C";
            return true;
        }
    }

    auto humIt = readings.find("air_humidity");
    if (humIt != readings.end() && humIt->second > m_humidityMax) {
        reason = "Humidity too HIGH: " + std::to_string(humIt->second) + " %";
        return true;
    }

    return false;
}

std::map<std::string, int> TemperatureEmergencyStrategy::filterDangerousCommands(const std::map<std::string, int>& commands, const std::map<std::string, double>& readings) const
{

    auto filtered = commands;
    auto tempIt = readings.find("temperature");

    if (tempIt != readings.end()) {
        // Если температура близка к критической (в пределах 5°C от границы),
        // блокируем потенциально опасные команды
        if (tempIt->second > m_tempMax - 5.0 && filtered["heater"] > 0) {
            filtered["heater"] = 0;
            std::cout << "[SafetyProxy]: Blocked heater - temperature near critical ("
                << tempIt->second << " C)" << std::endl;
        }
        if (tempIt->second < m_tempMin + 5.0 && filtered["conditioner"] > 0) {
            filtered["conditioner"] = 0;
            std::cout << "[SafetyProxy]: Blocked conditioner - temperature near critical ("
                << tempIt->second << " C)" << std::endl;
        }
    }

    return filtered;
}

void TemperatureEmergencyStrategy::setThresholds(double tempMin, double tempMax, double humidityMax)
{
    m_tempMin = tempMin;
    m_tempMax = tempMax;
    m_humidityMax = humidityMax;
}
