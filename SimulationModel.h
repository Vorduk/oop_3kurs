#pragma once
#include "IDataProvider.h"
#include <random>

class SimulationModel : public IDataProvider {

public:
    SimulationModel(double initial_temperature, double initial_air_humidity, double initial_soil_moisture);

    // Имплементация IDataProvider
    double getTemperature() const override { return m_temperature; }
    double getAirHumidity() const override { return m_air_humidity; }
    double getSoilMoisture() const override { return m_soil_moisture; }

    // Обновление состояния модели (один шаг)
    void update();

    // Для отладки
    void printParameters() const;

private:
    // Параметры
    double m_temperature;
    double m_air_humidity;
    double m_soil_moisture;

    // Генераторы случайных чисел
    std::mt19937 m_rng;
    std::uniform_real_distribution<> m_noise;

};