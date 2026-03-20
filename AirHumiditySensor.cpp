#include "AirHumiditySensor.h"

double AirHumiditySensor::getValue()
{
    if (m_dataProvider) {
        return m_dataProvider->getAirHumidity();
    }
    return 0.0;
}
