#include "TemperatureSensor.h"

double TemperatureSensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        double value = provider->getTemperature();
        std::cout << "[Sensor]: " << getType() << " [ID:" << getId() << "] Reading: " << value << " C" << std::endl;

        return value;
    }

    // Если провайдер умер, возвращается в начальное состояние
    return START_TEMPERATURE_READING;
}
