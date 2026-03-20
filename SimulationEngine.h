#pragma once
#include "ISimulationEngine.h"
#include "IOManager.h"
#include "SimulationModel.h"
#include "IClimateManager.h"
#include "IConfigManager.h"
#include "TimerSchedule.h"
#include <memory>
#include <map>
#include <string>

class ExtendedConfigManager;

/**
 * @brief Движок симуляции системы управления теплицей
 *
 * Центральный компонент, реализующий основной цикл управления.
 * Координирует работу всех остальных компонентов:
 * Сбор показаний с датчиков (через IOManager)
 * Получение целевых параметров (через IConfigManager)
 * Расчёт управляющих команд (через IClimateManager)
 * Применение команд к устройствам (через IOManager)
 * Обновление модели симуляции
 */
class SimulationEngine : public ISimulationEngine {
public:
    /**
     * @brief Конструктор движка симуляции
     * @param model Модель симуляции (источник данных для датчиков)
     * @param io_manager Менеджер ввода/вывода (датчики и устройства)
     */
    SimulationEngine(std::shared_ptr<SimulationModel> model,
        std::shared_ptr<IOManager> io_manager);

    ~SimulationEngine();

    /**
     * @brief Запустить основной цикл управления
     *
     * Запускает бесконечный цикл, в котором:
     * Считываются показания всех датчиков
     * Вычисляются управляющие команды
     * Команды применяются к устройствам
     * Модель симуляции обновляется
     * Пауза 2 секунды
     */
    void start() override;

    /**
     * @brief Остановить цикл управления
     *
     * Сигнализирует о завершении работы и дожидается
     * остановки всех фоновых таймеров (расписаний)
     */
    void stop() override;

    /**
     * @brief Установить менеджер климата (внедрение зависимости)
     * @param manager Умный указатель на менеджер климата
     *
     * Позволяет подменить реальный менеджер на прокси
     * для добавления проверок безопасности без изменения кода движка.
     */
    void setClimateManager(std::shared_ptr<IClimateManager> manager);

    /**
     * @brief Установить менеджер конфигурации (внедрение зависимости)
     * @param manager Умный указатель на менеджер конфигурации
     */
    void setConfigManager(std::shared_ptr<IConfigManager> manager);

    /**
     * @brief Настроить периодические задачи (расписания)
     * @param configManager Расширенный менеджер конфигурации с расписаниями
     *
     * Создаёт фоновые таймеры для устройств, которые должны работать
     * по расписанию (вентиляция, лампы).
     */
    void setupSchedules(ExtendedConfigManager* configManager);

private:
    /**
     * @brief Применить управляющие команды к устройствам и модели
     * @param commands Ассоциативный массив "устройство -> мощность"
     *
     * Отправляет команды устройствам через IOManager
     * Применяет эффекты к модели симуляции (изменение параметров)
     */
    void applyCommands(const std::map<std::string, int>& commands);

    /**
     * @brief Выполнить команду по расписанию
     * @param deviceType Тип устройства (ventilation, lamp)
     * @param powerLevel Уровень мощности (0-100%)
     *
     * Вызывается из фонового потока таймера.
     */
    void applyScheduledCommand(const std::string& deviceType, int powerLevel);

    bool m_is_running;                                      ///< Флаг работы цикла управления
    std::shared_ptr<SimulationModel> m_model;               ///< Модель симуляции
    std::shared_ptr<IOManager> m_io_manager;                ///< Менеджер ввода/вывода

    std::shared_ptr<IClimateManager> m_climateManager;      ///< Менеджер климата (может быть прокси)
    std::shared_ptr<IConfigManager> m_configManager;        ///< Менеджер конфигурации

    std::map<std::string, std::unique_ptr<TimerSchedule>> m_schedules;  ///< Активные таймеры расписаний
};