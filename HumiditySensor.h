#pragma once
#include "ISensor.h"

// Сенсор влажности.
class HumiditySensor : public ISensor
{
public:

private:
	double m_start_humidity = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};

