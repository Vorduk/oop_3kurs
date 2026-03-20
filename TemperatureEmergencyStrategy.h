#include "IEmergencyStrategy.h"
#include <string>

#pragma once
/**
 * @brief Конкретная стратегия для температурно-влажностных аварий
 *
 * Проверяет выход температуры и влажности за критические пороги.
 * При аварии выключает нагреватели, включает вентиляцию и кондиционер.
 */
class TemperatureEmergencyStrategy : public IEmergencyStrategy {
public:
    TemperatureEmergencyStrategy(double tempMin, double tempMax, double humidityMax);

    std::map<std::string, int> getEmergencyCommands() const override;

    bool isEmergency(const std::map<std::string, double>& readings, std::string& reason) const override;

    std::map<std::string, int> filterDangerousCommands(
        const std::map<std::string, int>& commands,
        const std::map<std::string, double>& readings) const override;

    std::string getName() const override { return "TemperatureEmergencyStrategy"; }

    void setThresholds(double tempMin, double tempMax, double humidityMax);

private:
    double m_tempMin;
    double m_tempMax;
    double m_humidityMax;
};


