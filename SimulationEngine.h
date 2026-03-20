#pragma once
#include "ISimulationEngine.h"
#include "IOManager.h"
#include "SimulationModel.h"
#include "IClimateManager.h"
#include "IConfigManager.h"
#include <memory>

// Управляет циклом обновления
class SimulationEngine : public ISimulationEngine {
public:
    SimulationEngine(SimulationModel& model, IOManager& io_manager);
    ~SimulationEngine();

    void start() override;
    void stop() override;

    void setClimateManager(std::shared_ptr<IClimateManager> manager);
    void setConfigManager(std::shared_ptr<IConfigManager> manager);

private:
    bool m_is_running;
    SimulationModel& m_model;
    IOManager& m_io_manager;

    std::shared_ptr<IClimateManager> m_climateManager;
    std::shared_ptr<IConfigManager> m_configManager;

    void applyCommands(const std::map<std::string, int>& commands);
};

