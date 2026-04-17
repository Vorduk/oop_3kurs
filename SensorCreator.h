#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <memory>

/**
 * @brief Абстрактный создатель датчиков (Creator в паттерне Factory Method)
 *
 * Объявляет фабричный метод createSensor(), который возвращает объект
 * типа ISensor. Конкретные создатели переопределяют этот метод.
 */
class SensorCreator {
public:
    virtual ~SensorCreator() = default;

    /**
     * @brief Фабричный метод для создания датчика
     * @param data_provider Источник данных для датчика
     * @return Умный указатель на созданный датчик
     */
    virtual std::shared_ptr<ISensor> createSensor(
        std::weak_ptr<const IDataProvider> data_provider) = 0;
};