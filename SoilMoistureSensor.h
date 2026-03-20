#pragma once
#include "ISensor.h"

// Cенсор влажности почвы.
class SoilMoistureSensor : public ISensor
{
public:

private:
	double m_start_moisture = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};

