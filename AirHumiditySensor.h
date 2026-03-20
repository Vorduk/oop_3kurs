#pragma once
#include "BaseSensor.h"


// Сенсор влажности.
class AirHumiditySensor : public BaseSensor
{
public:
    AirHumiditySensor(const IDataProvider* dataProvider)
        : BaseSensor(dataProvider, "air_humidity") {
    }
private:
	double m_start_humidity = 0; // Начальное показание датчика
	double getValue() override; // Метод для получения показания с датчика.
};
