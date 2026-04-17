#pragma once
#include <memory>
#include "IDevice.h"
#include "IRegulator.h"

/**
 * @brief Интерфейс абстрактной фабрики для создания семейств устройств и регуляторов
 *
 * Определяет методы для создания всех типов исполнительных устройств
 * и алгоритмов регулирования, используемых в системе управления теплицей.
 * Конкретные фабрики реализуют эти методы, возвращая соответствующие продукты.
 */
class IGreenhouseFactory {
public:
    virtual ~IGreenhouseFactory() = default;

    // Создание исполнительных устройств
    virtual std::shared_ptr<IDevice> createHeater() = 0;
    virtual std::shared_ptr<IDevice> createConditioner() = 0;
    virtual std::shared_ptr<IDevice> createAirHumidifier() = 0;
    virtual std::shared_ptr<IDevice> createIrrigation() = 0;
    virtual std::shared_ptr<IDevice> createVentilation() = 0;
    virtual std::shared_ptr<IDevice> createLamp() = 0;

    // Создание регуляторов
    virtual std::shared_ptr<IRegulator> createTemperatureRegulator() = 0;
    virtual std::shared_ptr<IRegulator> createHumidityRegulator() = 0;
    virtual std::shared_ptr<IRegulator> createSoilMoistureRegulator() = 0;
};