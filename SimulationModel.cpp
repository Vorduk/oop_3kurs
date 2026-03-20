#include "SimulationModel.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Конструктор модели симуляции
 *
 * Инициализирует начальные значения параметров и генератор случайных чисел.
 * Все эффекты устройств изначально равны 0.
 */
SimulationModel::SimulationModel(double initial_temperature,
    double initial_air_humidity,
    double initial_soil_moisture)
    : m_temperature(initial_temperature)
    , m_air_humidity(initial_air_humidity)
    , m_soil_moisture(initial_soil_moisture)
    , m_heater_effect(0.0)
    , m_conditioner_effect(0.0)
    , m_humidifier_effect(0.0)
    , m_irrigation_effect(0.0)
    , m_ventilation_effect(0.0)
    , m_lamp_effect(0.0)
    , m_humidity_effect_from_vent(0.0)
    , m_rng(std::random_device{}())          // Аппаратный энтропийный источник
    , m_noise(-0.3, 0.3) {                   // Случайные колебания ±0.3

    std::cout << "[INFO]: SimulationModel created" << std::endl;
}

/**
 * @brief Применить эффект нагревателя
 * @param powerLevel Мощность нагревателя (0-100%)
 *
 * Коэффициент преобразования: 0.025°C на 1% мощности
 * При 100% мощности: +2.5°C за цикл
 */
void SimulationModel::applyHeaterEffect(int powerLevel) {
    m_heater_effect = powerLevel * 0.025;
}

/**
 * @brief Применить эффект кондиционера
 * @param powerLevel Мощность кондиционера (0-100%)
 *
 * Коэффициент преобразования: -0.02°C на 1% мощности
 * При 100% мощности: -2.0°C за цикл
 *
 * @note Отрицательное значение, так как кондиционер охлаждает
 */
void SimulationModel::applyConditionerEffect(int powerLevel) {
    m_conditioner_effect = -powerLevel * 0.02;
}

/**
 * @brief Применить эффект увлажнителя воздуха
 * @param powerLevel Мощность увлажнителя (0-100%)
 *
 * Коэффициент преобразования: +0.015% на 1% мощности
 * При 100% мощности: +1.5% влажности за цикл
 */
void SimulationModel::applyHumidifierEffect(int powerLevel) {
    m_humidifier_effect = powerLevel * 0.015;
}

/**
 * @brief Применить эффект системы полива
 * @param powerLevel Интенсивность полива (0-100%)
 *
 * Коэффициент преобразования: +0.09% на 1% мощности
 * При 100% мощности: +9.0% влажности почвы за цикл
 *
 * @note Полив имеет наибольший эффект среди всех устройств,
 *       так как влажность почвы меняется быстрее воздуха
 */
void SimulationModel::applyIrrigationEffect(int powerLevel) {
    m_irrigation_effect = powerLevel * 0.09;
}

/**
 * @brief Применить эффект вентиляции
 * @param powerLevel Мощность вентиляции (0-100%)
 *
 * Вентиляция одновременно влияет на два параметра:
 * - Температура: -0.015°C на 1% мощности
 * - Влажность воздуха: -0.01% на 1% мощности
 *
 * При 100% мощности: -1.5°C и -1.0% влажности
 */
void SimulationModel::applyVentilationEffect(int powerLevel) {
    m_ventilation_effect = -powerLevel * 0.015;           // Охлаждение
    m_humidity_effect_from_vent = -powerLevel * 0.01;     // Осушение
}

/**
 * @brief Применить эффект ламп освещения
 * @param powerLevel Мощность ламп (0-100%)
 *
 * Коэффициент преобразования: +0.02°C на 1% мощности
 * При 100% мощности: +2.0°C за цикл
 *
 * @note Лампы не влияют на влажность
 */
void SimulationModel::applyLampEffect(int powerLevel) {
    m_lamp_effect = powerLevel * 0.02;
}

/**
 * @brief Сброс всех накопленных эффектов
 *
 * Вызывается после применения эффектов в update().
 * Это необходимо, чтобы эффекты не накапливались между циклами.
 */
void SimulationModel::resetDeviceEffects() {
    m_heater_effect = 0.0;
    m_conditioner_effect = 0.0;
    m_humidifier_effect = 0.0;
    m_irrigation_effect = 0.0;
    m_ventilation_effect = 0.0;
    m_lamp_effect = 0.0;
    m_humidity_effect_from_vent = 0.0;
}

/**
 * @brief Применить накопленные эффекты к параметрам модели
 *
 * Суммирует все эффекты устройств и добавляет их к текущим параметрам.
 * Вызывается из update() перед естественными процессами.
 */
void SimulationModel::applyDeviceEffects() {
    // Температура: нагреватель + кондиционер + вентиляция + лампы
    m_temperature += m_heater_effect + m_conditioner_effect
        + m_ventilation_effect + m_lamp_effect;

    // Влажность воздуха: увлажнитель + вентиляция
    m_air_humidity += m_humidifier_effect + m_humidity_effect_from_vent;

    // Влажность почвы: только полив
    m_soil_moisture += m_irrigation_effect;
}

/**
 * @brief Ограничение параметров допустимыми пределами
 *
 * Имитация физических ограничений
 * Температура не может опуститься ниже 10°C (опасность замерзания)
 * Температура не может подняться выше 35°C (перегрев растений)
 * Влажность воздуха: 30-90% (ниже — сухо, выше — плесень)
 * Влажность почвы: 20-80% (ниже — засуха, выше — загнивание корней)
 */
void SimulationModel::clampParameters() {
    const double MIN_TEMP = 10.0;
    const double MAX_TEMP = 35.0;
    const double MIN_HUMIDITY = 30.0;
    const double MAX_HUMIDITY = 90.0;
    const double MIN_SOIL = 20.0;
    const double MAX_SOIL = 80.0;

    m_temperature = std::clamp(m_temperature, MIN_TEMP, MAX_TEMP);
    m_air_humidity = std::clamp(m_air_humidity, MIN_HUMIDITY, MAX_HUMIDITY);
    m_soil_moisture = std::clamp(m_soil_moisture, MIN_SOIL, MAX_SOIL);
}

/**
 * @brief Обновление состояния модели на один шаг симуляции
 *
 * Порядок
 * applyDeviceEffects()  — применить эффекты от устройств
 * Добавить случайный шум — имитация естественных колебаний
 * Естественное испарение — снижение влажности почвы и воздуха
 * clampParameters() — ограничить параметры
 * resetDeviceEffects() — сбросить эффекты для следующего цикла
 *
 * @note Важен порядок: сначала устройства, потом естественные процессы,
 *       чтобы эффекты не "съедались" испарением в том же цикле
 */
void SimulationModel::update() {
    // Шаг 1: Применение эффектов от устройств
    applyDeviceEffects();

    // Шаг 2: Естественные процессы (случайные колебания)
    // Температура колеблется в пределах ±0.3°C
    m_temperature += m_noise(m_rng);
    // Влажность воздуха колеблется в пределах ±0.15%
    m_air_humidity += m_noise(m_rng) * 0.5;
    // Влажность почвы колеблется в пределах ±0.09%
    m_soil_moisture += m_noise(m_rng) * 0.3;

    // Шаг 3: Естественное испарение и охлаждение
    // Почва теряет влагу даже без полива
    m_soil_moisture -= 0.2;
    // Воздух теряет влагу естественным путём
    m_air_humidity -= 0.1;
    // Температура естественным образом не меняется (нет источников)

    // Шаг 4: Ограничение допустимыми пределами
    clampParameters();

    // Шаг 5: Сброс эффектов для следующего цикла
    resetDeviceEffects();
}

/**
 * @brief Вывод текущих параметров в консоль
 *
 * Используется для визуального контроля работы системы
 * в лабе. Вызывается каждый цикл управления.
 */
void SimulationModel::printParameters() const {
    std::cout << "\n=== Current greenhouse parameters values ===" << std::endl;
    std::cout << "Temperature: " << m_temperature << " C" << std::endl;
    std::cout << "Air humidity: " << m_air_humidity << " %" << std::endl;
    std::cout << "Soil moisture: " << m_soil_moisture << " %" << std::endl;
    std::cout << std::endl;
}