#pragma once
#include "ClimateAnalyzer.h"
#include <vector>
#include <memory>

/**
 * @brief Пул объектов ClimateAnalyzer (Singleton).
 *
 * Управляет фиксированным количеством заранее созданных анализаторов.
 * Клиенты могут получать объект во временное пользование и обязаны
 * возвращать его обратно в пул.
 */
class AnalyzerPool {
public:
    /**
     * @brief Получить единственный экземпляр пула.
     */
    static AnalyzerPool& getInstance();

    // Запрет копирования и перемещения
    AnalyzerPool(const AnalyzerPool&) = delete;
    AnalyzerPool& operator=(const AnalyzerPool&) = delete;

    /**
     * @brief Получить свободный анализатор.
     * @return Указатель на ClimateAnalyzer или nullptr, если все заняты.
     */
    ClimateAnalyzer* acquire();

    /**
     * @brief Вернуть анализатор в пул.
     * @param analyzer Указатель, ранее полученный через acquire().
     */
    void release(ClimateAnalyzer* analyzer);

    /**
     * @brief Получить текущее количество свободных анализаторов.
     */
    size_t availableCount() const;

private:
    AnalyzerPool(size_t poolSize = 3);
    ~AnalyzerPool() = default;

    size_t m_poolSize;
    std::vector<std::unique_ptr<ClimateAnalyzer>> m_allObjects;  // Все созданные объекты
    std::vector<ClimateAnalyzer*> m_freeObjects;                 // Свободные объекты
};