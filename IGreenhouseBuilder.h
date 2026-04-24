#pragma once
#include <memory>
#include "SimulationEngine.h"

// @brief Абстрактный строитель
class IGreenhouseBuilder {
public:
    virtual ~IGreenhouseBuilder() = default;

    // Создать модель симуляции
    virtual void buildModel() = 0;

    // Создать и добавить датчики
    virtual void buildSensors() = 0;

    // Создать исполнительные устройства с декораторами
    virtual void buildDevices() = 0;

    // Создать менеджер ввода/вывода
    virtual void buildIOManager() = 0;

    // Создать менеджер климата с регуляторами и прокси
    virtual void buildClimateManager() = 0;

    // Создать конфигурацию целевых параметров и расписаний
    virtual void buildConfig() = 0;

    // Собрать движок симуляции из готовых частей
    virtual void buildEngine() = 0;

    // Вернуть готовый к запуску движок
    virtual std::shared_ptr<SimulationEngine> getResult() = 0;
};