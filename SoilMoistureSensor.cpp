#include "SoilMoistureSensor.h"

double SoilMoistureSensor::getValue()
{
    if (m_data_provider) {
        return m_data_provider->getSoilMoisture();
    }
    return 0.0;
}
