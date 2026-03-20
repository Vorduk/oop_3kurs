#include "AirHumiditySensor.h"

double AirHumiditySensor::getValue() {
    // Проверка против висячих указателей: lock() возвращает shared_ptr или пустой (data provider - weak ptr)
    if (auto provider = m_data_provider.lock()) {
        return provider->getAirHumidity();;
    }
    // Если провайдер умер, возвращается 0
    return 0;
}
