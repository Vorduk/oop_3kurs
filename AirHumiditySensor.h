#pragma once
#include "BaseSensor.h"

#define START_AIR_HUMIDITY_READING 0

/**
 * @brief Датчик влажности воздуха
 *
 * Получает текущее значение влажности воздуха от IDataProvider.
 */
class AirHumiditySensor : public BaseSensor {
public:
    /**
     * @brief Конструктор датчика влажности воздуха
     * @param data_provider Умный указатель на источник данных
     */
    AirHumiditySensor(std::weak_ptr<const IDataProvider> data_provider);
    
    std::shared_ptr<ISensor> clone() const override;
private:
    /**
     * @brief Получить текущее показание влажности воздуха
     * @return Влажность в процентах (0-100%)
     */
	double getValue() override; // Метод для получения показания с датчика.

    
};
