#include "TemperatureSensor.h"

double TemperatureSensor::getValue()
{
    if (m_dataProvider) {
        return m_dataProvider->getTemperature();
    }
    return 0.0;
}
