#pragma once
#include "BaseSensor.h"

// Сенсор температуры
class TemperatureSensor : public BaseSensor
{
public:
    TemperatureSensor(const IDataProvider* dataProvider)
        : BaseSensor(dataProvider, "temperature") {
    }
private:
	double m_start_temperature = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};
