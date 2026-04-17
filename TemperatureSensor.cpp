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

TemperatureSensor::TemperatureSensor(std::weak_ptr<const IDataProvider> data_provider)
    : BaseSensor(data_provider, "temperature")
{
}

// Реализация clone()
std::shared_ptr<ISensor> TemperatureSensor::clone() const {
    std::cout << "[TemperatureSensor] Cloning sensor ID=" << getId() << std::endl;
    // Используется конструктор копирования TemperatureSensor
    return std::make_shared<TemperatureSensor>(*this);
}