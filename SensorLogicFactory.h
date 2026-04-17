#pragma once
#include "ISensorLogic.h"
#include "IDataProvider.h"
#include <map>
#include <memory>
#include <string>

/**
 * @brief Фабрика приспособленцев (FlyweightFactory)
 *
 * Для каждого сочетания (тип, провайдер) создаётся только один объект.
 */
class SensorLogicFactory {
public:
    /**
     * @brief Получить разделяемую логику датчика
     * @param type Тип датчика ("temperature", "air_humidity", "soil_moisture")
     * @param provider Источник данных
     * @return Умный указатель на разделяемую логику
     */
    std::shared_ptr<ISensorLogic> getSensorLogic(
        const std::string& type,
        std::weak_ptr<const IDataProvider> provider);

private:
    std::string makeKey(const std::string& type, std::weak_ptr<const IDataProvider> provider); // Ключ для кэширования

    std::map<std::string, std::shared_ptr<ISensorLogic>> m_cache;  ///< Кэш приспособленцев
};