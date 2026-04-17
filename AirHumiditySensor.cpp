#include "AirHumiditySensor.h"

double AirHumiditySensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        double value = provider->getAirHumidity();;
        std::cout << "[Sensor]: " << getType() << " [ID:" << getId() << "] Reading: " << value << " %" << std::endl;

        return value;
    }
    // Если провайдер умер, возвращается в начальное состояние
    return START_AIR_HUMIDITY_READING;
}

AirHumiditySensor::AirHumiditySensor(std::weak_ptr<const IDataProvider> data_provider)
    : BaseSensor(data_provider, "air_humidity")
{
}

std::shared_ptr<ISensor> AirHumiditySensor::clone() const {
    std::cout << "[AirHumiditySensor] Cloning sensor ID=" << getId() << std::endl;
    // Используется конструктор копирования
    return std::make_shared<AirHumiditySensor>(*this);
}