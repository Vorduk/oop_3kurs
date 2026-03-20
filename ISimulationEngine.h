#pragma once

/**
 * @brief Интерфейс движка симуляции системы управления теплицей
 *
 * Определяет управление циклом обновления системы.
 */
class ISimulationEngine {
public:
    virtual ~ISimulationEngine() = default;

    /**
     * @brief Запустить цикл управления
     */
    virtual void start() = 0;

    /**
     * @brief Остановить цикл управления
     */
    virtual void stop() = 0;
};
