#pragma once
#include "ISensor.h"
#include "ISensorLogic.h"
#include <memory>
#include <string>

/**
 * @brief Лёгкий датчик (UnsharedConcreteFlyweight)
 *
 * Хранит только внешнее состояние (уникальный ID) и ссылку на разделяемую логику.
 * Все операции делегирует приспособленцу.
 */
class LightweightSensor : public ISensor {
public:
    /**
     * @brief Конструктор
     * @param logic Разделяемая логика (приспособленец)
     */
    explicit LightweightSensor(std::shared_ptr<ISensorLogic> logic);

    double getValue() override;
    std::string getType() const override;
    int getId() const override;

private:
    int m_id;                                   ///< Состояние (уникальное)
    std::shared_ptr<ISensorLogic> m_logic;      ///< Ссылка на разделяемую логику
    static int s_next_id;                       ///< Статический счётчик для генерации ID
};