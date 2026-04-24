#pragma once
#include "IParameterObserver.h"
#include <string>

// Наблюдатель, который выводит в консоль информацию об изменении параметров.
class LoggingObserver : public IParameterObserver {
public:
    LoggingObserver();

    void onTemperatureChanged(double oldValue, double newValue) override;
    void onAirHumidityChanged(double oldValue, double newValue) override;
    void onSoilMoistureChanged(double oldValue, double newValue) override;
};