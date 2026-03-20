#include "TemperatureSensor.h"

double TemperatureSensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        return provider->getTemperature();
    }
    // Если провайдер умер, возвращается в начальное состояние
    return START_TEMPERATURE_READING;
}
