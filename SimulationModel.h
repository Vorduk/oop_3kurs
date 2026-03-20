#pragma once
#include "IDataProvider.h"
#include <random>
#include <map>
#include <string>
#include <memory>

class SimulationModel : public IDataProvider {
public:
    SimulationModel(double initial_temperature, double initial_air_humidity, double initial_soil_moisture);

    double getTemperature() const override { return m_temperature; }
    double getAirHumidity() const override { return m_air_humidity; }
    double getSoilMoisture() const override { return m_soil_moisture; }

    void update();
    void printParameters() const;

    void applyHeaterEffect(int powerLevel);
    void applyConditionerEffect(int powerLevel);
    void applyHumidifierEffect(int powerLevel);
    void applyIrrigationEffect(int powerLevel);
    void applyVentilationEffect(int powerLevel);
    void applyLampEffect(int powerLevel);

private:
    double m_temperature;
    double m_air_humidity;
    double m_soil_moisture;

    double m_heater_effect;
    double m_conditioner_effect;
    double m_humidifier_effect;
    double m_irrigation_effect;
    double m_ventilation_effect;
    double m_lamp_effect;
    double m_humidity_effect_from_vent;

    std::mt19937 m_rng;
    std::uniform_real_distribution<> m_noise;

    void applyDeviceEffects();
    void resetDeviceEffects();
    void clampParameters();
};