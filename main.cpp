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
#include "OldHeaterAdapter.h"
#include "DeviceLoggerDecorator.h"
#include "DevicePowerLimitDecorator.h"
#include "TemperatureSensorsComposite.h"
#include "SimulatedDeviceDriver.h"
#include "ConsoleDeviceDriver.h"

int main() {
    // Вывод заголовка
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Модель — источник данных для датчиков
    std::shared_ptr<SimulationModel> simulation_model = std::make_shared<SimulationModel>(18.0, 65.0, 45.0);

    // Создание множества датчиков
    std::vector<std::shared_ptr<BaseSensor>> all_sensors;
    std::cout << "\nCreating sensors..." << std::endl;

    // Композит  датчиков температуры
    // Подкомпозит 1 (3 датчика температуры)
    std::shared_ptr<TemperatureSensorsComposite> subComposite1 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 3; ++i) {
        std::shared_ptr<TemperatureSensor> tempSensor = std::make_shared<TemperatureSensor>(simulation_model);
        subComposite1->addSensor(tempSensor);
    }

    // Подкомпозит 2 (2 датчика температуры)
    std::shared_ptr<TemperatureSensorsComposite> subComposite2 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<TemperatureSensor> tempSensor = std::make_shared<TemperatureSensor>(simulation_model);
        subComposite2->addSensor(tempSensor);
    }

    // Композит из этих двух композитов
    std::shared_ptr<TemperatureSensorsComposite> rootComposite = std::make_shared<TemperatureSensorsComposite>();
    rootComposite->addSensor(subComposite1);
    rootComposite->addSensor(subComposite2);

    // Добавление в общий список датчиков
    all_sensors.push_back(rootComposite);

    // 2 датчика влажности воздуха
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr <BaseSensor> sensor = std::make_shared<AirHumiditySensor>(simulation_model);
        all_sensors.push_back(sensor);
        std::cout << "Created sensor: Type='" << sensor->getType() << "', ID=" << sensor->getId() << std::endl;
    }

    // 2 датчика влажности почвы
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr <BaseSensor> sensor = std::make_shared<SoilMoistureSensor>(simulation_model);
        all_sensors.push_back(sensor);
        std::cout << "Created sensor: Type='" << sensor->getType() << "', ID=" << sensor->getId() << std::endl;
    }

    std::cout << "\nTotal sensors created: " << all_sensors.size() << std::endl;

    // Подсчет датчиков по типам для вывода
    int tempCount = 0, humidityCount = 0, soilCount = 0;
    for (const std::shared_ptr <BaseSensor>& sensor : all_sensors) {
        if (sensor->getType() == "temperature") tempCount++;
        else if (sensor->getType() == "air_humidity") humidityCount++;
        else if (sensor->getType() == "soil_moisture") soilCount++;
    }
    std::cout << "  - Temperature sensors: " << tempCount << std::endl;
    std::cout << "  - Air humidity sensors: " << humidityCount << std::endl;
    std::cout << "  - Soil moisture sensors: " << soilCount << std::endl;

    // Создание устройств
    std::cout << "\nCreating devices..." << std::endl;

    std::vector<std::shared_ptr<BaseDevice>> allDevices;

    std::shared_ptr<BaseDevice> humidifier = std::make_shared<AirHumidifier>();
    allDevices.push_back(humidifier);
    std::cout << "  Created device: Type='" << humidifier->getType() << "', ID=" << humidifier->getId() << std::endl;

    std::shared_ptr<BaseDevice> irrigation = std::make_shared<Irrigation>();
    allDevices.push_back(irrigation);
    std::cout << "  Created device: Type='" << irrigation->getType() << "', ID=" << irrigation->getId() << std::endl;

    std::shared_ptr<BaseDevice> ventilation = std::make_shared<Ventilation>();
    allDevices.push_back(ventilation);
    std::cout << "  Created device: Type='" << ventilation->getType() << "', ID=" << ventilation->getId() << std::endl;

    std::shared_ptr<BaseDevice> lamp = std::make_shared<Lamp>();
    allDevices.push_back(lamp);
    std::cout << "  Created device: Type='" << lamp->getType() << "', ID=" << lamp->getId() << std::endl;

    // IOManager
    std::cout << "\nInitializing IOManager..." << std::endl;
    std::shared_ptr<IOManager> io_manager = std::make_shared<IOManager>();

    // Bridge

    // Реализации
    std::shared_ptr<SimulatedDeviceDriver> sim_driver_heater = std::make_shared<SimulatedDeviceDriver>();
    std::shared_ptr<ConsoleDeviceDriver> console_driver = std::make_shared<ConsoleDeviceDriver>("Conditioner-01");

    // Абстракции
    std::shared_ptr<BaseDevice> heater = std::make_shared<Heater>(sim_driver_heater);
    std::shared_ptr<BaseDevice> conditioner = std::make_shared<Conditioner>(console_driver);

    // Регистрация без изменений
    io_manager->addDevice(heater);
    io_manager->addDevice(conditioner);

    // Регистрация всех датчиков из единого массива
    std::cout << "\nRegistering all sensors..." << std::endl;
    for (std::shared_ptr<BaseSensor>& sensor : all_sensors) {
        io_manager->addSensor(sensor);
    }

    // Регистрация всех устройств
    std::cout << "\nRegistering all devices..." << std::endl;
    for (std::shared_ptr<BaseDevice>& device : allDevices) {
        io_manager->addDevice(device);
    }

    // Реальный менеджер климата
    std::cout << "\nCreating ClimateManager with regulators..." << std::endl;
    std::shared_ptr<ClimateManager> realClimateManager = std::make_shared<ClimateManager>();

    // Создание регуляторов
    std::shared_ptr<PidRegulator> pidRegulator = std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
    std::shared_ptr<OnOffRegulator> onOffRegulator = std::make_shared<OnOffRegulator>(2.0);

    // Назначение регуляторов параметрам
    realClimateManager->setRegulator("temperature", pidRegulator);
    realClimateManager->setRegulator("air_humidity", onOffRegulator);
    realClimateManager->setRegulator("soil_moisture", onOffRegulator);

    // Создание стратегии аварийного реагирования
    std::shared_ptr<IEmergencyStrategy> emergencyStrategy = std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);

    // Прокси – теперь все настройки через конструктор
    std::shared_ptr<IClimateManager> safetyProxy = std::make_shared<SafetyProxyManager>(
        realClimateManager,
        io_manager,
        emergencyStrategy
    );

    // Конфигурация
    std::cout << "\nCreating ExtendedConfigManager..." << std::endl;
    std::shared_ptr<ExtendedConfigManager> configManager = std::make_shared<ExtendedConfigManager>();

    // Установка целевых параметров
    double target_temperature = 23.0;
    double target_air_humidity = 65.0;
    double target_soil_moisture = 50.0;
    configManager->setTargetParameter("temperature", target_temperature);
    configManager->setTargetParameter("air_humidity", target_air_humidity);
    configManager->setTargetParameter("soil_moisture", target_soil_moisture);

    std::cout << "\nTarget parameters set:" << std::endl;
    std::cout << "  - Temperature target: " << target_temperature << "C" << std::endl;
    std::cout << "  - Air humidity target: " << target_air_humidity << "%" << std::endl;
    std::cout << "  - Soil moisture target: " << target_soil_moisture << " %" << std::endl;

    // Настройка периодических задач
    configManager->setSchedule("ventilation", 10, 50);
    configManager->setSchedule("lamp", 15, 70);

    std::cout << "\nSchedules configured:" << std::endl;
    std::cout << "  - Ventilation: every 10 seconds at 50% power" << std::endl;
    std::cout << "  - Lamp: every 15 seconds at 70% power" << std::endl;

    // Движок симуляции
    std::cout << "\nCreating SimulationEngine..." << std::endl;
    SimulationEngine engine(simulation_model, io_manager);

    // Конфигурирование движка
    engine.setClimateManager(safetyProxy);
    engine.setConfigManager(configManager);
    engine.setupSchedules(configManager.get());

    // Итоговая конфигурация
    std::cout << "\n========================================" << std::endl;
    std::cout << "System configured with:" << std::endl;
    std::cout << "Sensors:" << std::endl;
    std::cout << "  - Total sensors: " << all_sensors.size() << std::endl;
    std::cout << "  - Temperature sensors: " << tempCount << std::endl;
    std::cout << "  - Air humidity sensors: " << humidityCount << std::endl;
    std::cout << "  - Soil moisture sensors: " << soilCount << std::endl;

    std::cout << "\nDevices:" << std::endl;
    for (const std::shared_ptr<BaseDevice>& device : allDevices) {
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

    // Запуск основного цикла управления
    engine.start();

    return 0;
}