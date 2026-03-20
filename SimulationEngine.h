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

class ExtendedConfigManager;

class SimulationEngine : public ISimulationEngine {
public:
    // Изменяем сигнатуру: принимаем shared_ptr на модель и shared_ptr на IOManager
    SimulationEngine(std::shared_ptr<SimulationModel> model,
        std::shared_ptr<IOManager> io_manager);  // ← было IOManager&

    ~SimulationEngine();

    void start() override;
    void stop() override;

    void setClimateManager(std::shared_ptr<IClimateManager> manager);
    void setConfigManager(std::shared_ptr<IConfigManager> manager);
    void setupSchedules(ExtendedConfigManager* configManager);

private:
    bool m_is_running;
    std::shared_ptr<SimulationModel> m_model;
    std::shared_ptr<IOManager> m_io_manager;  // ← было IOManager&, теперь shared_ptr

    std::shared_ptr<IClimateManager> m_climateManager;
    std::shared_ptr<IConfigManager> m_configManager;

    std::map<std::string, std::unique_ptr<TimerSchedule>> m_schedules;

    void applyCommands(const std::map<std::string, int>& commands);
    void applyScheduledCommand(const std::string& deviceType, int powerLevel);
};