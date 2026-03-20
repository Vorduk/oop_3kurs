#pragma once
#include "ISimulationEngine.h"
#include "SimulationModel.h"

// Управляет циклом обновления
class SimulationEngine : public ISimulationEngine
{
public:
	SimulationEngine(SimulationModel& model);
	~SimulationEngine();
	void start() override;
	void stop() override;
private:
	bool m_is_running;
	SimulationModel& m_model;
};

