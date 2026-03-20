#pragma once
#include "IDevice.h"
#include <string>

/**
 * @brief Абстрактный базовый класс для всех устройств
 *
 * Реализует общую функциональность: хранение типа и ID.
 * Конкретные устройства наследуются от этого класса.
 */
class BaseDevice : public IDevice {
public:
    /**
     * @brief Конструктор базового устройства
     * @param type Строковый тип устройства
     */
    BaseDevice(const std::string& type);
    virtual ~BaseDevice() = default;

    /**
     * @brief Получить тип устройства
     * @return Строковый тип устройства
     */
    std::string getType() const override;

    /**
     * @brief Получить уникальный идентификатор
     * @return ID устройства
     */
    int getId() const override;

protected:
    std::string m_type;      ///< Тип устройства
    int m_id;                ///< Уникальный идентификатор
    static int s_next_id;    ///< Статический счетчик для генерации ID
};

