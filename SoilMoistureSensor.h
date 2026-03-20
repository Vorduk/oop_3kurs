#pragma once
#include "BaseSensor.h"

#define START_SOIL_MOISTURE_READING 0

/**
 * @brief Датчик влажности почвы
 *
 * Получает текущее значение влажности почвы от IDataProvider.
 */
class SoilMoistureSensor : public BaseSensor {
public:
    /**
     * @brief Конструктор датчика влажности почвы
     * @param data_provider Умный указатель на источник данных
     */
    SoilMoistureSensor(std::weak_ptr<const IDataProvider> data_provider)
        : BaseSensor(data_provider, "soil_moisture") {
    }
private:
    /**
     * @brief Получить текущее показание влажности почвы
     * @return Влажность в процентах (0-100%)
     */
	double getValue() override; // Метод для получения показания с датчика.
};
