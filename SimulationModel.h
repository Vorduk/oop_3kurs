#pragma once
#include "IDataProvider.h"
#include <random>
#include <map>
#include <string>
#include <memory>

/**
 * @brief Модель симуляции физических процессов в теплице
 *
 * Реализует интерфейс IDataProvider, предоставляя текущие значения
 * параметров микроклимата датчикам.
 *
 * Модель симулирует
 * Изменение температуры под действием нагревателей и кондиционеров
 * Изменение влажности воздуха от увлажнителей и вентиляции
 * Изменение влажности почвы от системы полива
 * Естественные процессы (охлаждение, испарение, случайные флуктуации)
 *
 * Эффекты от устройств накапливаются в отдельных полях (m_heater_effect и т.д.)
 * и применяются единовременно при вызове update().
 *
 * @note Использует подход "накопления эффектов": устройства вызывают
 *       applyXxxEffect(), а update() суммирует все эффекты и применяет их.
 *       Это позволяет корректно обрабатывать команды от нескольких устройств.
 */
class SimulationModel : public IDataProvider {
public:
    /**
     * @brief Конструктор модели симуляции
     * @param initial_temperature Начальная температура воздуха (°C)
     * @param initial_air_humidity Начальная влажность воздуха (%)
     * @param initial_soil_moisture Начальная влажность почвы (%)
     */
    SimulationModel(double initial_temperature, double initial_air_humidity,
        double initial_soil_moisture);

    // Реализация интерфейса IDataProvider
    double getTemperature() const override { return m_temperature; }
    double getAirHumidity() const override { return m_air_humidity; }
    double getSoilMoisture() const override { return m_soil_moisture; }

    /**
     * @brief Обновление состояния модели на один шаг симуляции
     *
     * Выполняет
     * Применение накопленных эффектов от устройств
     * Добавление случайного шума (имитация естественных колебаний)
     * Естественное испарение и охлаждение
     * Ограничение параметров допустимыми пределами
     * Сброс накопленных эффектов
     *
     * @note Вызывается каждый цикл управления (раз в 2 секунды)
     */
    void update();

    /**
     * @brief Вывод текущих параметров в консоль
     *
     * Используется для мониторинга состояния системы в лабораторной работе
     */
    void printParameters() const;

    // Методы накопления эффектов от устройств (паттерн "накопление")
    void applyHeaterEffect(int powerLevel);         ///< Нагреватель (повышает температуру)
    void applyConditionerEffect(int powerLevel);    ///< Кондиционер (понижает температуру)
    void applyHumidifierEffect(int powerLevel);     ///< Увлажнитель (повышает влажность воздуха)
    void applyIrrigationEffect(int powerLevel);     ///< Полив (повышает влажность почвы)
    void applyVentilationEffect(int powerLevel);    ///< Вентиляция (понижает T и влажность)
    void applyLampEffect(int powerLevel);           ///< Лампы (повышают температуру)

private:
    // Текущие параметры модели
    double m_temperature;       ///< Температура воздуха (°C)
    double m_air_humidity;      ///< Влажность воздуха (%)
    double m_soil_moisture;     ///< Влажность почвы (%)

    // Накопленные эффекты от устройств (суммируются перед update())
    double m_heater_effect;                 ///< Влияние нагревателя на температуру
    double m_conditioner_effect;            ///< Влияние кондиционера на температуру
    double m_humidifier_effect;             ///< Влияние увлажнителя на влажность воздуха
    double m_irrigation_effect;             ///< Влияние полива на влажность почвы
    double m_ventilation_effect;            ///< Влияние вентиляции на температуру
    double m_lamp_effect;                   ///< Влияние ламп на температуру
    double m_humidity_effect_from_vent;     ///< Влияние вентиляции на влажность воздуха

    // Генераторы случайных чисел (имитация естественных колебаний)
    std::mt19937 m_rng;                         ///< Генератор псевдослучайных чисел
    std::uniform_real_distribution<> m_noise;   ///< Распределение шума (-0.3 .. +0.3)

    // Вспомогательные методы
    void applyDeviceEffects();  ///< Применить накопленные эффекты к параметрам
    void resetDeviceEffects();  ///< Обнулить накопленные эффекты
    void clampParameters();     ///< Ограничить параметры допустимыми пределами
};