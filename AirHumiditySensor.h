#pragma once
#include "BaseSensor.h"

// Сенсор влажности воздуха.
class AirHumiditySensor : public BaseSensor
{
public:
    AirHumiditySensor(const IDataProvider* data_provider)
        : BaseSensor(data_provider, "air_humidity") {
    }
private:
	double m_start_humidity = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};
