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

		std::map<int, double> allReadings = m_io_manager.readAllSensors();
		std::cout << "\n=== Readings from ALL sensors (by ID) ===" << std::endl;
		for (const auto& [id, value] : allReadings) {
			ISensor* sensor = m_io_manager.getSensor(id);
			std::cout << "Sensor [ID: " << id << ", Type: " << sensor->getType()
				<< "]: " << value;
			if (sensor->getType() == "temperature") std::cout << " C";
			if (sensor->getType() == "air_humidity") std::cout << " %";
			if (sensor->getType() == "soil_moisture") std::cout << " %";
			std::cout << std::endl;
		}
	}
}

void SimulationEngine::stop()
{
	m_is_running = false;
}



