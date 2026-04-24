#pragma once
#include "IParameterObserver.h"

// Наблюдатель, который выводит предупреждения при выходе параметров за безопасные границы.
class AlertObserver : public IParameterObserver {
public:
    AlertObserver(double minTemp, double maxTemp, double maxHumidity);

    void onTemperatureChanged(double oldValue, double newValue) override;
    void onAirHumidityChanged(double oldValue, double newValue) override;
    void onSoilMoistureChanged(double oldValue, double newValue) override;

private:
    double m_minTemp;
    double m_maxTemp;
    double m_maxHumidity;
};