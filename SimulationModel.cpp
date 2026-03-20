#include "SimulationModel.h"
#include <iostream>
#include <algorithm>

SimulationModel::SimulationModel(double initial_temperature, double initial_air_humidity, double initial_soil_moisture)
    : m_temperature(initial_temperature)
    , m_air_humidity(initial_air_humidity)
    , m_soil_moisture(initial_soil_moisture)
    , m_heater_effect(0.0)
    , m_conditioner_effect(0.0)
    , m_humidifier_effect(0.0)
    , m_irrigation_effect(0.0)
    , m_ventilation_effect(0.0)
    , m_lamp_effect(0.0)
    , m_humidity_effect_from_vent(0.0)
    , m_rng(std::random_device{}())
    , m_noise(-0.3, 0.3) {

    std::cout << "[INFO]: SimulationModel created" << std::endl;
}

void SimulationModel::applyHeaterEffect(int powerLevel) {
    m_heater_effect = powerLevel * 0.025;
}

void SimulationModel::applyConditionerEffect(int powerLevel) {
    m_conditioner_effect = -powerLevel * 0.02;
}

void SimulationModel::applyHumidifierEffect(int powerLevel) {
    m_humidifier_effect = powerLevel * 0.015;
}

void SimulationModel::applyIrrigationEffect(int powerLevel) {
    m_irrigation_effect = powerLevel * 0.09;
}

void SimulationModel::applyVentilationEffect(int powerLevel) {
    // Вентиляция: снижает температуру и влажность
    m_ventilation_effect = -powerLevel * 0.015;
    m_humidity_effect_from_vent = -powerLevel * 0.01;
}

void SimulationModel::applyLampEffect(int powerLevel) {
    // Лампы: повышают температуру
    m_lamp_effect = powerLevel * 0.02;
}

void SimulationModel::resetDeviceEffects() {
    m_heater_effect = 0.0;
    m_conditioner_effect = 0.0;
    m_humidifier_effect = 0.0;
    m_irrigation_effect = 0.0;
    m_ventilation_effect = 0.0;
    m_lamp_effect = 0.0;
    m_humidity_effect_from_vent = 0.0;
}

void SimulationModel::applyDeviceEffects() {
    m_temperature += m_heater_effect + m_conditioner_effect + m_ventilation_effect + m_lamp_effect;
    m_air_humidity += m_humidifier_effect + m_humidity_effect_from_vent;
    m_soil_moisture += m_irrigation_effect;
}

void SimulationModel::clampParameters() {
    const double MIN_TEMP = 10.0;
    const double MAX_TEMP = 35.0;
    const double MIN_HUMIDITY = 30.0;
    const double MAX_HUMIDITY = 90.0;
    const double MIN_SOIL = 20.0;
    const double MAX_SOIL = 80.0;

    m_temperature = std::clamp(m_temperature, MIN_TEMP, MAX_TEMP);
    m_air_humidity = std::clamp(m_air_humidity, MIN_HUMIDITY, MAX_HUMIDITY);
    m_soil_moisture = std::clamp(m_soil_moisture, MIN_SOIL, MAX_SOIL);
}

void SimulationModel::update() {
    applyDeviceEffects();

    // Естественные процессы
    m_temperature += m_noise(m_rng);
    m_air_humidity += m_noise(m_rng) * 0.5;
    m_soil_moisture += m_noise(m_rng) * 0.3;

    // Естественное испарение и охлаждение
    m_soil_moisture -= 0.2;
    m_air_humidity -= 0.1;

    clampParameters();
    resetDeviceEffects();
}

void SimulationModel::printParameters() const {
    std::cout << "\n=== Current greenhouse parameters values ===" << std::endl;
    std::cout << "Temperature: " << m_temperature << " C" << std::endl;
    std::cout << "Air humidity: " << m_air_humidity << " %" << std::endl;
    std::cout << "Soil moisture: " << m_soil_moisture << " %" << std::endl;
    std::cout << std::endl;
}