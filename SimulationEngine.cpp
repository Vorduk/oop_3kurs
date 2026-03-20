#include "SimulationEngine.h"

SimulationEngine::SimulationEngine(SimulationModel& model) :
	m_model(model),
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
	}
}

void SimulationEngine::stop()
{
	m_is_running = false;
}
