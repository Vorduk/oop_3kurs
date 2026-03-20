#pragma once
#include "BaseSensor.h"

// Cенсор влажности почвы.
class SoilMoistureSensor : public BaseSensor
{
public:
    SoilMoistureSensor(const IDataProvider* dataProvider)
        : BaseSensor(dataProvider, "soil_moisture") {
    }
private:
	double m_start_moisture = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};
