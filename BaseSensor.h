#pragma once
#include "ISensor.h"
#include "IDataProvider.h"
#include <string>

/**
 * @brief Абстрактный базовый класс для всех датчиков
 *
 * Реализует общую функциональность: хранение типа, ID, ссылки на источник данных.
 * Конкретные датчики наследуются от этого класса и реализуют метод getValue().
 */
class BaseSensor : public ISensor {

protected:
    std::weak_ptr<const IDataProvider> m_data_provider;   ///< Невладеющая ссылка на источник данных (модель симуляции)
    std::string m_type;         ///< Тип датчика (temperature, air_humidity, soil_moisture)
    int m_id;                   ///< Уникальный идентификатор датчика
    static int s_next_id;       ///< Статический счетчик для генерации ID

public:
    /**
     * @brief Конструктор базового датчика
     * @param data_provider Указатель на источник данных (не может быть nullptr)
     * @param type Строковый тип датчика
     */
    BaseSensor(std::weak_ptr<const IDataProvider> data_provider, const std::string& type);

    /**
     * @brief Получить тип датчика
     * @return Строковый тип датчика
     */
    std::string getType() const override;

    /**
     * @brief Получить уникальный идентификатор
     * @return ID датчика
     */
    int getId() const override;

    virtual ~BaseSensor() = default;
};

