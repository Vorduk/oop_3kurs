#pragma once
#include <string>
#include <iostream>

/**
 * @brief Тяжеловесный анализатор климатических данных.
 *
 * Имитирует дорогое создание. Объекты этого класса предназначены
 * для повторного использования через пул объектов.
 */
class ClimateAnalyzer {
public:
    ClimateAnalyzer();
    ~ClimateAnalyzer();

    /**
     * @brief Сброс состояния перед возвратом в пул.
     *
     * Обнуляет накопленные данные, чтобы следующий клиент
     * получил "чистый" объект.
     */
    void reset();

    /**
     * @brief Выполнить анализ текущих показаний.
     * @param temperature Текущая температура
     * @param humidity Текущая влажность воздуха
     * @param soilMoisture Текущая влажность почвы
     */
    std::string analyze(double temperature, double humidity, double soilMoisture);

    /**
     * @brief Получить уникальный идентификатор экземпляра.
     */
    int getId() const { return m_id; }

private:
    int m_id;                       ///< Уникальный ID для логирования
    static int s_next_id;            ///< Счётчик ID
    int m_analysis_count;            ///< Счётчик вызовов (сбрасывается в reset)
};