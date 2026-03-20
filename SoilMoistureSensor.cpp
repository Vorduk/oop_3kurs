#include "SoilMoistureSensor.h"

double SoilMoistureSensor::getValue()
{
    if (m_dataProvider) {
        return m_dataProvider->getSoilMoisture();
    }
    return 0.0;
}
