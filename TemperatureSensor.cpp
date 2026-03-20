#include "TemperatureSensor.h"

double TemperatureSensor::getValue()
{
    if (m_data_provider) {
        return m_data_provider->getTemperature();
    }
    return 0.0;
}
