#pragma once
#include "IGreenhouseBuilder.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "ClimateManager.h"
#include "SafetyProxyManager.h"
#include "ScheduleConfig.h"
#include "ISensor.h"
#include "IDevice.h"
#include <memory>
#include <vector>

class DefaultGreenhouseBuilder : public IGreenhouseBuilder {
public:
    DefaultGreenhouseBuilder();
    ~DefaultGreenhouseBuilder() override = default;

    void buildModel() override;
    void buildSensors() override;
    void buildDevices() override;
    void buildIOManager() override;
    void buildClimateManager() override;
    void buildConfig() override;
    void buildEngine() override;

    std::shared_ptr<SimulationEngine> getResult() override;

private:
    std::shared_ptr<SimulationEngine> m_engine; // Продукт

    std::shared_ptr<SimulationModel> m_simulation_model;
    std::shared_ptr<IOManager> m_io_manager;
    std::shared_ptr<ClimateManager> m_real_climate_manager;
    std::shared_ptr<IClimateManager> m_safety_proxy;
    std::shared_ptr<ExtendedConfigManager> m_config_manager;

    std::vector<std::shared_ptr<ISensor>> m_sensors;
    std::vector<std::shared_ptr<IDevice>> m_devices;
};