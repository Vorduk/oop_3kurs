#pragma once
#include "IGreenhouseFactory.h"

/**
 * @brief Фабрика стандартной конфигурации теплицы
 *
 * Создаёт базовые устройства без декораторов и регуляторы
 * для всех параметров микроклимата.
 */
class GreenhouseFactory : public IGreenhouseFactory {
public:
    GreenhouseFactory();
    ~GreenhouseFactory() override = default;

    std::shared_ptr<IDevice> createHeater() override;
    std::shared_ptr<IDevice> createConditioner() override;
    std::shared_ptr<IDevice> createAirHumidifier() override;
    std::shared_ptr<IDevice> createIrrigation() override;
    std::shared_ptr<IDevice> createVentilation() override;
    std::shared_ptr<IDevice> createLamp() override;

    std::shared_ptr<IRegulator> createTemperatureRegulator() override;
    std::shared_ptr<IRegulator> createHumidityRegulator() override;
    std::shared_ptr<IRegulator> createSoilMoistureRegulator() override;
};