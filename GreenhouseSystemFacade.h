#pragma once

#include <memory>
#include <string>
#include <vector>
#include "BaseDevice.h"

class SimulationModel;
class IOManager;
class IClimateManager;
class ExtendedConfigManager;
class SimulationEngine;
class BaseSensor;
class BaseDevice;

/**
 * @brief Фасад системы управления теплицей.
 *
 * Интерфейс для инициализации, конфигурирования и запуска
 * всех подсистем. Клиент (main) взаимодействует только с этим классом,
 * не зная о внутреннем устройстве системв.
 */
class GreenhouseSystemFacade {
public:
    /**
     * @brief Конструктор фасада.
     *
     * Выполняет:
     * - создание модели симуляции;
     * - создание датчиков и устройств;
     * - настройку менеджеров и регуляторов;
     * - сборку движка симуляции.
     */
    GreenhouseSystemFacade();

    ~GreenhouseSystemFacade();

    /**
     * @brief Установить целевое значение параметра микроклимата.
     * @param parameter Имя параметра ("temperature", "air_humidity", "soil_moisture")
     * @param value Целевое значение (температура в °C, влажность в %)
     */
    void setTargetParameter(const std::string& parameter, double value);

    /**
     * @brief Настроить расписание периодической задачи.
     * @param deviceType Тип устройства ("ventilation", "lamp")
     * @param intervalSeconds Интервал в секундах
     * @param powerLevel Мощность (0-100)
     */
    void setSchedule(const std::string& deviceType, int intervalSeconds, int powerLevel);

    /**
     * @brief Главный цикл управления теплицей.
     */
    void start();

    void stop();

private:
    /**
     * @brief Инициализация всех подсистем.
     */
    void initialize();

    /**
     * @brief Создание датчиков и регистрация их.
     */
    void createSensors();

    /**
     * @brief Создание исполнительных устройств и их регистрация.
     */
    void createDevices();

    /**
     * @brief Настройка менеджера климата и регуляторов.
     */
    void setupClimateManager();

private:
    // Компоненты системы
    std::shared_ptr<SimulationModel>      m_model;
    std::shared_ptr<IOManager>            m_io_manager;
    std::shared_ptr<IClimateManager>      m_climate_manager;
    std::shared_ptr<ExtendedConfigManager> m_config_manager;
    std::unique_ptr<SimulationEngine>     m_engine;

    // Контейнеры для управления временем жизни датчиков и устройств
    std::vector<std::shared_ptr<BaseSensor>> m_sensors;
    std::vector<std::shared_ptr<BaseDevice>> m_devices;

    // Признак инициализации
    bool m_initialized;
};