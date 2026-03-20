#include "SoilMoistureSensor.h"

double SoilMoistureSensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        return provider->getSoilMoisture();
    }
    // Если провайдер умер, возвращается в начальное состояние
    return START_SOIL_MOISTURE_READING;
}
