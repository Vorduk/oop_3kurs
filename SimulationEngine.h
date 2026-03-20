#pragma once
#include "ISimulationEngine.h"
#include "IOManager.h"
#include "SimulationModel.h"
#include "IClimateManager.h"
#include "IConfigManager.h"
#include "TimerSchedule.h"
#include <memory>
#include <map>
#include <string>

class ExtendedConfigManager; // Forward declaration

class SimulationEngine : public ISimulationEngine {
public:
    SimulationEngine(SimulationModel& model, IOManager& io_manager);
    ~SimulationEngine();

    void start() override;
    void stop() override;

    void setClimateManager(std::shared_ptr<IClimateManager> manager);
    void setConfigManager(std::shared_ptr<IConfigManager> manager);

    void setupSchedules(ExtendedConfigManager* configManager);

private:
    bool m_is_running;
    SimulationModel& m_model;
    IOManager& m_io_manager;

    std::shared_ptr<IClimateManager> m_climateManager;
    std::shared_ptr<IConfigManager> m_configManager;

    std::map<std::string, std::unique_ptr<TimerSchedule>> m_schedules;

    void applyCommands(const std::map<std::string, int>& commands);
    void applyScheduledCommand(const std::string& deviceType, int powerLevel);
};