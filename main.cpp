#include <iostream>
#include <memory>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"
#include "ClimateManager.h"
#include "ScheduleConfig.h"  // Здесь находится ExtendedConfigManager
#include "SafetyProxyManager.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"

int main() {
    // Вывод заголовка системы
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System - Lab 4" << std::endl;
    std::cout << "Patterns: Delegation, Proxy, Configuration" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Модель — источник данных для датчиков (температура, влажность воздуха, влажность почвы)
    std::shared_ptr<SimulationModel> simulationModel =
        std::make_shared<SimulationModel>(22.0, 65.0, 45.0);

    // Датчики получают данные через weak_ptr на модель (предотвращает циклические ссылки)
    std::shared_ptr<TemperatureSensor> tempSensor =
        std::make_shared<TemperatureSensor>(simulationModel);
    std::shared_ptr<AirHumiditySensor> humiditySensor =
        std::make_shared<AirHumiditySensor>(simulationModel);
    std::shared_ptr<SoilMoistureSensor> soilSensor =
        std::make_shared<SoilMoistureSensor>(simulationModel);

    // Нагреватель — повышает температуру
    std::shared_ptr<Heater> heater = std::make_shared<Heater>();
    // Кондиционер — понижает температуру
    std::shared_ptr<Conditioner> conditioner = std::make_shared<Conditioner>();
    // Увлажнитель — повышает влажность воздуха
    std::shared_ptr<AirHumidifier> humidifier = std::make_shared<AirHumidifier>();
    // Полив — повышает влажность почвы
    std::shared_ptr<Irrigation> irrigation = std::make_shared<Irrigation>();
    // Вентиляция — понижает температуру и влажность
    std::shared_ptr<Ventilation> ventilation = std::make_shared<Ventilation>();
    // Лампы — повышают температуру, обеспечивают освещение
    std::shared_ptr<Lamp> lamp = std::make_shared<Lamp>();

    // IOManager хранит все датчики и устройства, управляет командами
    std::shared_ptr<IOManager> io_manager = std::make_shared<IOManager>();

    // Регистрация датчиков
    io_manager->addSensor(tempSensor);
    io_manager->addSensor(humiditySensor);
    io_manager->addSensor(soilSensor);

    // Регистрация устройств
    io_manager->addDevice(heater);
    io_manager->addDevice(conditioner);
    io_manager->addDevice(humidifier);
    io_manager->addDevice(irrigation);
    io_manager->addDevice(ventilation);
    io_manager->addDevice(lamp);

    // Реальный менеджер — вычисляет управляющие сигналы
    std::shared_ptr<ClimateManager> realClimateManager =
        std::make_shared<ClimateManager>();

    // Создание регуляторов
    std::shared_ptr<PidRegulator> pidRegulator =
        std::make_shared<PidRegulator>(2.0, 0.1, 0.5);   // ПИД-регулятор для температуры
    std::shared_ptr<OnOffRegulator> onOffRegulator =
        std::make_shared<OnOffRegulator>(2.0);            // On/Off для влажности

    // Назначение регуляторов параметрам (делегирование вычислений)
    realClimateManager->setRegulator("temperature", pidRegulator);
    realClimateManager->setRegulator("air_humidity", onOffRegulator);
    realClimateManager->setRegulator("soil_moisture", onOffRegulator);

    std::cout << "\n[Delegation Pattern]: ClimateManager delegates calculations to:" << std::endl;
    std::cout << "  - Temperature -> PID Regulator" << std::endl;
    std::cout << "  - Humidity -> On/Off Regulator with hysteresis" << std::endl;
    std::cout << "  - Soil moisture -> On/Off Regulator with hysteresis" << std::endl;

    // SafetyProxyManager оборачивает реальный менеджер, добавляя проверки безопасности
    std::shared_ptr<SafetyProxyManager> safetyProxy =
        std::make_shared<SafetyProxyManager>(realClimateManager, io_manager);

    // Стратегия аварийного реагирования (температурно-влажностные пороги)
    std::shared_ptr<TemperatureEmergencyStrategy> emergencyStrategy =
        std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);
    safetyProxy->setEmergencyStrategy(emergencyStrategy);

    std::cout << "\n[Proxy Pattern]: SafetyProxyManager wraps ClimateManager" << std::endl;
    std::cout << "  - Emergency thresholds: temp [5-40]C, humidity <90%" << std::endl;

    // ExtendedConfigManager хранит целевые параметры и расписания для устройств
    std::shared_ptr<ExtendedConfigManager> configManager =
        std::make_shared<ExtendedConfigManager>();

    // Установка целевых параметров микроклимата
    configManager->setTargetParameter("temperature", 23.0);
    configManager->setTargetParameter("air_humidity", 65.0);
    configManager->setTargetParameter("soil_moisture", 50.0);

    // Настройка периодических задач (паттерн Configuration)
    configManager->setSchedule("ventilation", 10, 50);   // Вентиляция каждые 10 сек на 50%
    configManager->setSchedule("lamp", 15, 70);          // Лампы каждые 15 сек на 70%

    std::cout << "\n[Configuration Pattern]: Schedules configured:" << std::endl;
    std::cout << "  - Ventilation: every 10s at 50%" << std::endl;
    std::cout << "  - Lamp: every 15s at 70%" << std::endl;

    // SimulationEngine — центральный компонент, запускающий цикл управления
    SimulationEngine engine(simulationModel, io_manager);

    // Конфигурирование движка (внедрение зависимостей)
    engine.setClimateManager(safetyProxy);      // Используем прокси вместо реального менеджера
    engine.setConfigManager(configManager);      // Менеджер конфигурации
    engine.setupSchedules(configManager.get());  // Настройка таймеров по расписаниям

    // Вывод итоговой конфигурации
    std::cout << "\n========================================" << std::endl;
    std::cout << "[INFO]: System fully configured" << std::endl;
    std::cout << "  - Target temperature: 23.0 C" << std::endl;
    std::cout << "  - Target humidity: 65.0 %" << std::endl;
    std::cout << "  - Target soil moisture: 50.0 %" << std::endl;
    std::cout << "\n[INFO]: Starting control system...\n" << std::endl;

    // Запуск основного цикла управления
    engine.start();

    return 0;
}