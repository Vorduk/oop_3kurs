#pragma once
#include "BaseSensor.h"

#define START_TEMPERATURE_READING 0

/**
 * @brief Датчик температуры воздуха
 *
 * Получает текущее значение температуры от IDataProvider.
 */
class TemperatureSensor : public BaseSensor {
public:
    /**
     * @brief Конструктор датчика температуры
     * @param data_provider Умный указатель на источник данных
     */
    TemperatureSensor(std::weak_ptr<const IDataProvider> data_provider);
    std::shared_ptr<ISensor> clone() const override;
private:

    /**
     * @brief Получить текущее показание температуры
     * @return Температура в градусах Цельсия
     */
	double getValue() override; // Метод для получения показания с датчика.

    
};
