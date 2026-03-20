#include "AirHumiditySensor.h"

double AirHumiditySensor::getValue()
{
    if (m_data_provider) {
        return m_data_provider->getAirHumidity();
    }
    return 0.0;
}
