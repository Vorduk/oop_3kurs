#include "SimulationEngine.h"
#include <iostream>

SimulationEngine::SimulationEngine(SimulationModel& model, IOManager& io_manager) :
	m_model(model),
	m_io_manager(io_manager),
	m_is_running(false)
{

}

SimulationEngine::~SimulationEngine()
{
	stop();
}

void SimulationEngine::start()
{
	m_is_running = true;

	// Главный цикл
	while (m_is_running)
	{
		m_model.update(); // Обновление модели.
		m_model.printParameters();

		std::map<std::string, double> readings = m_io_manager.readAllSensors();
		std::cout << "\n=== Current greenhouse parameters from sensors ===" << std::endl;
		for (std::map<std::string, double>::const_iterator it = readings.begin();
			it != readings.end(); ++it) {
			std::cout << it->first << ": " << it->second;
			if (it->first == "temperature") std::cout << " C";
			if (it->first == "air_humidity") std::cout << " %";
			if (it->first == "soil_moisture") std::cout << " %";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void SimulationEngine::stop()
{
	m_is_running = false;
}
