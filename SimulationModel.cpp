#include "SimulationModel.h"
#include <iostream>
#include <algorithm>

SimulationModel::SimulationModel(double initial_temperature, double initial_air_humidity, double initial_soil_moisture)
    : m_temperature(initial_temperature)
    , m_air_humidity(initial_air_humidity)
    , m_soil_moisture(initial_soil_moisture)
    , m_rng(std::random_device{}())
    , m_noise(-0.5, 0.5) {

    std::cout << "[INFO]: SimulationModel created" << std::endl;
}

void SimulationModel::update() {
    const std::pair<double, double> TEMP_RANGE = { 10.0, 35.0 };
    const std::pair<double, double> HUMIDITY_RANGE = { 30.0, 90.0 };
    const std::pair<double, double> SOIL_RANGE = { 20.0, 80.0 };

    // Случайные изменения параметров
    m_temperature += m_noise(m_rng);
    m_air_humidity += m_noise(m_rng) * 0.5;
    m_soil_moisture += m_noise(m_rng) * 0.3;

    // Ограничение значений
    m_temperature = std::clamp(m_temperature, TEMP_RANGE.first, TEMP_RANGE.second);
    m_air_humidity = std::clamp(m_air_humidity, HUMIDITY_RANGE.first, HUMIDITY_RANGE.second);
    m_soil_moisture = std::clamp(m_soil_moisture, SOIL_RANGE.first, SOIL_RANGE.second);
}

void SimulationModel::printParameters() const {
    std::cout << "\n=== Current greenhouse parameters values ===" << std::endl;
    std::cout << "Temperature: " << m_temperature << " C" << std::endl;
    std::cout << "Air humidity: " << m_air_humidity << " %" << std::endl;
    std::cout << "Soil moisture: " << m_soil_moisture << " %" << std::endl;
    std::cout << std::endl;
}