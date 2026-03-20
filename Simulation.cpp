#include "Simulation.h"

Simulation::Simulation(double initial_temperature, double initial_air_humidity, double initial_soil_moisture) : 
	m_temperature(initial_temperature), 
    m_air_humidity(initial_air_humidity),
	m_soil_moisture(initial_soil_moisture)
{
	std::cout << "[INFO]: Simulation created" << std::endl;
}

void Simulation::start()
{
	m_is_running = true;
	std::cout << "[INFO]: Simulation started" << std::endl;

	while (m_is_running) {
		update();
        printParameters();
	}
}

void Simulation::stop()
{
    m_is_running = false;
    std::cout << "[INFO]: Simulation stopped" << std::endl;
}

void Simulation::update()
{
    // Максимальные и минимальные значения
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

void Simulation::printParameters()
{
    std::cout << "\n=== Current greenhouse parameters values ===" << std::endl;
    std::cout << "Temperature: " << m_temperature << " C" << std::endl;
    std::cout << "Air humidity: " << m_air_humidity << " %" << std::endl;
    std::cout << "Soil moisture: " << m_soil_moisture << " %" << std::endl;
    std::cout << std::endl << std::endl;
}
