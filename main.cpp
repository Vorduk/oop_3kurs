#include <iostream>
#include <memory>
#include <vector>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"
#include "ClimateManager.h"
#include "ScheduleConfig.h"
#include "SafetyProxyManager.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"
#include "RemoteControl.h"
#include "TurnOnCommand.h"
#include "TurnOffCommand.h"
#include "SetPowerCommand.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    std::shared_ptr<SimulationModel> simulation_model = std::make_shared<SimulationModel>(-50.0, 65.0, 45.0);

    std::vector<std::shared_ptr<BaseSensor>> all_sensors;
    std::cout << "\nCreating sensors..." << std::endl;

    for (int i = 0; i < 5; ++i) {
        std::shared_ptr<TemperatureSensor> temp_sensor = std::make_shared<TemperatureSensor>(simulation_model);
        all_sensors.push_back(temp_sensor);
        std::cout << "Created sensor: Type='" << temp_sensor->getType() << "', ID=" << temp_sensor->getId() << std::endl;
    }

    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<AirHumiditySensor> humidity_sensor = std::make_shared<AirHumiditySensor>(simulation_model);
        all_sensors.push_back(humidity_sensor);
        std::cout << "Created sensor: Type='" << humidity_sensor->getType() << "', ID=" << humidity_sensor->getId() << std::endl;
    }

    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<SoilMoistureSensor> soil_sensor = std::make_shared<SoilMoistureSensor>(simulation_model);
        all_sensors.push_back(soil_sensor);
        std::cout << "Created sensor: Type='" << soil_sensor->getType() << "', ID=" << soil_sensor->getId() << std::endl;
    }

    std::cout << "\nTotal sensors created: " << all_sensors.size() << std::endl;

    int temp_count = 0, humidity_count = 0, soil_count = 0;
    for (const std::shared_ptr<BaseSensor>& sensor : all_sensors) {
        if (sensor->getType() == "temperature") temp_count++;
        else if (sensor->getType() == "air_humidity") humidity_count++;
        else if (sensor->getType() == "soil_moisture") soil_count++;
    }
    std::cout << "  - Temperature sensors: " << temp_count << std::endl;
    std::cout << "  - Air humidity sensors: " << humidity_count << std::endl;
    std::cout << "  - Soil moisture sensors: " << soil_count << std::endl;

    std::cout << "\nCreating devices..." << std::endl;

    std::vector<std::shared_ptr<BaseDevice>> all_devices;

    std::shared_ptr<BaseDevice> heater = std::make_shared<Heater>();
    all_devices.push_back(heater);
    std::cout << "  Created device: Type='" << heater->getType() << "', ID=" << heater->getId() << std::endl;

    std::shared_ptr<BaseDevice> conditioner = std::make_shared<Conditioner>();
    all_devices.push_back(conditioner);
    std::cout << "  Created device: Type='" << conditioner->getType() << "', ID=" << conditioner->getId() << std::endl;

    std::shared_ptr<BaseDevice> humidifier = std::make_shared<AirHumidifier>();
    all_devices.push_back(humidifier);
    std::cout << "  Created device: Type='" << humidifier->getType() << "', ID=" << humidifier->getId() << std::endl;

    std::shared_ptr<BaseDevice> irrigation = std::make_shared<Irrigation>();
    all_devices.push_back(irrigation);
    std::cout << "  Created device: Type='" << irrigation->getType() << "', ID=" << irrigation->getId() << std::endl;

    std::shared_ptr<BaseDevice> ventilation = std::make_shared<Ventilation>();
    all_devices.push_back(ventilation);
    std::cout << "  Created device: Type='" << ventilation->getType() << "', ID=" << ventilation->getId() << std::endl;

    std::shared_ptr<BaseDevice> lamp = std::make_shared<Lamp>();
    all_devices.push_back(lamp);
    std::cout << "  Created device: Type='" << lamp->getType() << "', ID=" << lamp->getId() << std::endl;

    std::cout << "\nInitializing IOManager..." << std::endl;
    std::shared_ptr<IOManager> io_manager = std::make_shared<IOManager>();

    for (std::shared_ptr<BaseSensor>& sensor : all_sensors) {
        io_manager->addSensor(sensor);
    }

    for (std::shared_ptr<BaseDevice>& device : all_devices) {
        io_manager->addDevice(device);
    }

    std::cout << "\nCreating ClimateManager with regulators..." << std::endl;
    std::shared_ptr<ClimateManager> real_climate_manager = std::make_shared<ClimateManager>();

    std::shared_ptr<PidRegulator> pid_regulator = std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
    std::shared_ptr<OnOffRegulator> on_off_regulator = std::make_shared<OnOffRegulator>(2.0);

    real_climate_manager->setRegulator("temperature", pid_regulator);
    real_climate_manager->setRegulator("air_humidity", on_off_regulator);
    real_climate_manager->setRegulator("soil_moisture", on_off_regulator);

    std::shared_ptr<IEmergencyStrategy> emergency_strategy = std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);

    std::shared_ptr<IClimateManager> safety_proxy = std::make_shared<SafetyProxyManager>(
        real_climate_manager,
        io_manager,
        emergency_strategy
    );

    std::cout << "\nCreating ExtendedConfigManager..." << std::endl;
    std::shared_ptr<ExtendedConfigManager> config_manager = std::make_shared<ExtendedConfigManager>();

    double target_temperature = 23.0;
    double target_air_humidity = 65.0;
    double target_soil_moisture = 50.0;
    config_manager->setTargetParameter("temperature", target_temperature);
    config_manager->setTargetParameter("air_humidity", target_air_humidity);
    config_manager->setTargetParameter("soil_moisture", target_soil_moisture);

    std::cout << "\nTarget parameters set:" << std::endl;
    std::cout << "  - Temperature target: " << target_temperature << " C" << std::endl;
    std::cout << "  - Air humidity target: " << target_air_humidity << " %" << std::endl;
    std::cout << "  - Soil moisture target: " << target_soil_moisture << " %" << std::endl;

    config_manager->setSchedule("ventilation", 10, 50);
    config_manager->setSchedule("lamp", 15, 70);

    std::cout << "\nSchedules configured:" << std::endl;
    std::cout << "  - Ventilation: every 10 seconds at 50% power" << std::endl;
    std::cout << "  - Lamp: every 15 seconds at 70% power" << std::endl;

    std::cout << "\nCreating SimulationEngine..." << std::endl;
    SimulationEngine engine(simulation_model, io_manager);

    engine.setClimateManager(safety_proxy);
    engine.setConfigManager(config_manager);
    engine.setupSchedules(config_manager.get());

    std::cout << "\n========================================" << std::endl;
    std::cout << "System configured with:" << std::endl;
    std::cout << "Sensors:" << std::endl;
    std::cout << "  - Total sensors: " << all_sensors.size() << std::endl;
    std::cout << "  - Temperature sensors: " << temp_count << std::endl;
    std::cout << "  - Air humidity sensors: " << humidity_count << std::endl;
    std::cout << "  - Soil moisture sensors: " << soil_count << std::endl;

    std::cout << "\nDevices:" << std::endl;
    for (const std::shared_ptr<BaseDevice>& device : all_devices) {
        std::cout << "  - " << device->getType() << " (ID: " << device->getId() << ")" << std::endl;
    }

    std::cout << "\nTargets:" << std::endl;
    std::cout << "  - Temperature: 23.0 C" << std::endl;
    std::cout << "  - Air humidity: 65.0 %" << std::endl;
    std::cout << "  - Soil moisture: 50.0 %" << std::endl;

    std::cout << "\nSchedules:" << std::endl;
    std::cout << "  - Ventilation: every 10s at 50%" << std::endl;
    std::cout << "  - Lamp: every 15s at 70%" << std::endl;

    std::cout << "\nSafety:" << std::endl;
    std::cout << "  - Emergency thresholds: temp [5-40]C, humidity <90%" << std::endl;

    std::cout << "\n========================================" << std::endl;
    std::cout << "Starting main loop...\n" << std::endl;

    RemoteControl remote;

    std::shared_ptr<IDevice> heater_cmd = heater;
    std::shared_ptr<IDevice> conditioner_cmd = conditioner;
    std::shared_ptr<IDevice> humidifier_cmd = humidifier;
    std::shared_ptr<IDevice> irrigation_cmd = irrigation;
    std::shared_ptr<IDevice> ventilation_cmd = ventilation;
    std::shared_ptr<IDevice> lamp_cmd = lamp;

    remote.setCommand(1, std::make_shared<TurnOnCommand>(heater_cmd));
    remote.setCommand(2, std::make_shared<TurnOffCommand>(heater_cmd));
    remote.setCommand(3, std::make_shared<SetPowerCommand>(
        std::dynamic_pointer_cast<IAdjustableDevice>(heater_cmd), 75));
    remote.setCommand(4, std::make_shared<TurnOnCommand>(conditioner_cmd));
    remote.setCommand(5, std::make_shared<TurnOffCommand>(conditioner_cmd));
    remote.setCommand(6, std::make_shared<SetPowerCommand>(
        std::dynamic_pointer_cast<IAdjustableDevice>(conditioner_cmd), 60));
    remote.setCommand(7, std::make_shared<TurnOnCommand>(humidifier_cmd));
    remote.setCommand(8, std::make_shared<TurnOffCommand>(humidifier_cmd));
    remote.setCommand(9, std::make_shared<TurnOnCommand>(irrigation_cmd));
    remote.setCommand(10, std::make_shared<TurnOffCommand>(irrigation_cmd));
    remote.setCommand(11, std::make_shared<TurnOnCommand>(ventilation_cmd));
    remote.setCommand(12, std::make_shared<TurnOffCommand>(ventilation_cmd));
    remote.setCommand(13, std::make_shared<TurnOnCommand>(lamp_cmd));
    remote.setCommand(14, std::make_shared<TurnOffCommand>(lamp_cmd));

    std::cout << "\nCommand Pattern" << std::endl;
    remote.pressButton(1);
    remote.pressButton(3);
    remote.pressButton(4);
    remote.pressButton(6);
    remote.pressButton(7);
    remote.pressButton(9);
    remote.pressButton(11);
    remote.pressButton(13);
    remote.pressButton(2);
    remote.pressButton(5);
    remote.pressButton(8);
    remote.pressButton(10);
    remote.pressButton(12);
    remote.pressButton(14);
    std::cout << "End of Command\n" << std::endl;

    engine.start();

    return 0;
}