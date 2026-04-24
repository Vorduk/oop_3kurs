// GreenhouseDirector.h
#pragma once
#include "IGreenhouseBuilder.h"
#include <memory>

// Директор процесса сборки тепличной системы
class GreenhouseDirector {
public:

    GreenhouseDirector(std::shared_ptr<IGreenhouseBuilder> builder)
        : m_builder(builder) {
    }

    /**
     * Запустить процесс поэтапного конструирования системы
     *
     * Порядок шагов:
     * 1. Модель симуляции
     * 2. Датчики
     * 3. Исполнительные устройства
     * 4. Менеджер ввода/вывода
     * 5. Климатический менеджер с прокси
     * 6. Конфигурация (цели + расписания)
     * 7. Движок симуляции
     */
    void construct() {
        if (!m_builder) {
            throw std::runtime_error("Builder is not set");
        }
        m_builder->buildModel();
        m_builder->buildSensors();
        m_builder->buildDevices();
        m_builder->buildIOManager();
        m_builder->buildClimateManager();
        m_builder->buildConfig();
        m_builder->buildEngine();
    }

    // @brief Сменить строителя
    void setBuilder(std::shared_ptr<IGreenhouseBuilder> builder) {
        m_builder = builder;
    }

private:
    std::shared_ptr<IGreenhouseBuilder> m_builder;
};