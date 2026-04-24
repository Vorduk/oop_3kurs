#pragma once

// Интерфейс наблюдателя за изменениями климатических параметров.
class IParameterObserver {
public:
    virtual ~IParameterObserver() = default;

    // Вызывается при изменении температуры.
    virtual void onTemperatureChanged(double oldValue, double newValue) = 0;

    // Вызывается при изменении влажности воздуха.
    virtual void onAirHumidityChanged(double oldValue, double newValue) = 0;

    // Вызывается при изменении влажности почвы.
    virtual void onSoilMoistureChanged(double oldValue, double newValue) = 0;
};