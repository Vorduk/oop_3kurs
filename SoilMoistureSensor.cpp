#include "SoilMoistureSensor.h"

double SoilMoistureSensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        double value = provider->getSoilMoisture();
        std::cout << "[Sensor]: " << getType() << " [ID:" << getId() << "] Reading: " << value << " %" << std::endl;

        return value;
    }
    // Если провайдер умер, возвращается в начальное состояние
    return START_SOIL_MOISTURE_READING;
}

SoilMoistureSensor::SoilMoistureSensor(std::weak_ptr<const IDataProvider> data_provider)
    : BaseSensor(data_provider, "soil_moisture")
{
}

std::shared_ptr<ISensor> SoilMoistureSensor::clone() const {
    std::cout << "[SoilMoistureSensor] Cloning sensor ID=" << getId() << std::endl;
    // Используется конструктор копирования
    return std::make_shared<SoilMoistureSensor>(*this);
}