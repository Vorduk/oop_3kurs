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
    TemperatureSensor(std::weak_ptr<const IDataProvider> data_provider)
        : BaseSensor(data_provider, "temperature") {
    }
private:

    /**
     * @brief Получить текущее показание температуры
     * @return Температура в градусах Цельсия
     */
	double getValue() override; // Метод для получения показания с датчика.
};
