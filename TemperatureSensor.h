#pragma once
#include "ISensor.h"

// Сенсор температуры
class TemperatureSensor : public ISensor
{
public:

private:
	double m_start_temperature = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};

