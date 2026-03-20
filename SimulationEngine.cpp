#include "SimulationEngine.h"
#include "ScheduleConfig.h"
#include <iostream>
#include <thread>
#include <chrono>

/**
 * @brief Конструктор движка симуляции
 *
 * Инициализирует компоненты. Движок не запускается автоматически —
 * требуется явный вызов start() после конфигурации.
 */
SimulationEngine::SimulationEngine(std::shared_ptr<SimulationModel> model,
    std::shared_ptr<IOManager> io_manager)
    : m_model(model), m_io_manager(io_manager), m_is_running(false) {
}

SimulationEngine::~SimulationEngine() {
    stop();  // Остановка цикла и всех таймеров при разрушении
}

void SimulationEngine::setClimateManager(std::shared_ptr<IClimateManager> manager) {
    m_climateManager = manager;
}

void SimulationEngine::setConfigManager(std::shared_ptr<IConfigManager> manager) {
    m_configManager = manager;
}

/**
 * @brief Настройка периодических задач по расписанию
 *
 * Создаёт фоновые потоки для каждого устройства с расписанием.
 * Каждый таймер в отдельном потоке периодически вызывает
 * applyScheduledCommand() для своего устройства.
 */
void SimulationEngine::setupSchedules(ExtendedConfigManager* configManager) {
    if (!configManager || !m_io_manager) return;

    for (const auto& pair : configManager->getAllSchedules()) {
        const std::string& deviceType = pair.first;
        const ScheduleConfig& schedule = pair.second;

        if (!schedule.enabled) continue;

        // Создаём и запускаем таймер
        auto timer = std::make_unique<TimerSchedule>();
        timer->start(schedule.interval, [this, deviceType, schedule]() {
            // Лямбда-функция выполняемая в фоновом потоке
            if (m_is_running) {
                applyScheduledCommand(deviceType, schedule.powerLevel);
            }
            });

        m_schedules[deviceType] = std::move(timer);
        std::cout << "[Engine]: Timer started for " << deviceType
            << " every " << schedule.interval.count() << "s" << std::endl;
    }
}

/**
 * @brief Выполнение команды по расписанию
 *
 * Вызывается из фонового потока таймера. Применяет эффекты к модели,
 * которые будут учтены при следующем вызове update() в главном цикле.
 */
void SimulationEngine::applyScheduledCommand(const std::string& deviceType, int powerLevel) {
    if (!m_io_manager) return;

    std::cout << "\n[Timer]: Executing scheduled command for " << deviceType
        << " at " << powerLevel << "%" << std::endl;

    auto deviceIds = m_io_manager->getDeviceIdsByType(deviceType);
    for (int deviceId : deviceIds) {
        // Отправка команды мощности устройству
        m_io_manager->sendPowerCommand(deviceId, powerLevel);

        // Применение эффектов к модели (накапливаются до следующего update())
        if (deviceType == "ventilation" && m_model) {
            m_model->applyVentilationEffect(powerLevel);
        }
        else if (deviceType == "lamp" && m_model) {
            m_model->applyLampEffect(powerLevel);
        }
    }
}

/**
 * @brief Применение команд управления к устройствам и модели
 *
 * Вызывается из главного цикла управления. Эффекты от устройств
 * накапливаются и будут применены при следующем вызове update().
 */
void SimulationEngine::applyCommands(const std::map<std::string, int>& commands) {
    if (!m_model || !m_io_manager) return;

    for (const auto& pair : commands) {
        const std::string& deviceType = pair.first;
        int power = pair.second;

        auto deviceIds = m_io_manager->getDeviceIdsByType(deviceType);
        for (int deviceId : deviceIds) {
            // Отправляем команду мощности устройству
            m_io_manager->sendPowerCommand(deviceId, power);
        }

        // Применение эффектоа к модели (накапливаются до следующего update())
        if (deviceType == "heater") {
            m_model->applyHeaterEffect(power);
        }
        else if (deviceType == "conditioner") {
            m_model->applyConditionerEffect(power);
        }
        else if (deviceType == "air_humidifier") {
            m_model->applyHumidifierEffect(power);
        }
        else if (deviceType == "irrigation") {
            m_model->applyIrrigationEffect(power);
        }
    }
}

/**
 * @brief Основной цикл управления
 *
 * Работает в главном потоке.
 * Проверка наличия всех компонентов
 * Установка целевых параметров в ClimateManager
 * Бесконечный цикл (пока m_is_running == true):
 *    Чтение показаний всех датчиков
 *    Преобразование ID датчиков в имена параметров
 *    Вывод текущих показаний в консоль
 *    Расчёт управляющих команд (делегирование ClimateManager)
 *    Применение команд к устройствам и модели
 *    Обновление модели симуляции (естественные процессы + эффекты устройств)
 *    Вывод текущего состояния модели
 *    Пауза 2 секунды
 */
void SimulationEngine::start() {
    // Проверка: все компоненты должны быть установлены
    if (!m_climateManager || !m_configManager || !m_model || !m_io_manager) {
        std::cout << "[ERROR]: Required components not set!" << std::endl;
        std::cout << "  ClimateManager: " << (m_climateManager ? "OK" : "MISSING") << std::endl;
        std::cout << "  ConfigManager: " << (m_configManager ? "OK" : "MISSING") << std::endl;
        std::cout << "  Model: " << (m_model ? "OK" : "MISSING") << std::endl;
        std::cout << "  IOManager: " << (m_io_manager ? "OK" : "MISSING") << std::endl;
        return;
    }

    m_is_running = true;

    // Передча целевых параметров в менеджер климата
    auto targets = m_configManager->getAllTargets();
    m_climateManager->setTargetParameters(targets);

    std::cout << "\n[INFO]: SimulationEngine started with control loop" << std::endl;

    // Основной цикл управления
    while (m_is_running) {
        // Чтение показаний всех датчиков
        auto sensorReadings = m_io_manager->readAllSensors();

        // Преобразование ID датчиков в имена параметров
        std::map<std::string, double> currentValues;
        for (const auto& pair : sensorReadings) {
            int id = pair.first;
            double value = pair.second;
            auto sensor = m_io_manager->getSensor(id);
            if (sensor) {
                currentValues[sensor->getType()] = value;
            }
        }

        // Вывод текущих показаний
        std::cout << "\n=== Current readings ===" << std::endl;
        for (const auto& pair : currentValues) {
            const std::string& param = pair.first;
            double value = pair.second;
            std::cout << param << ": " << value;
            if (param == "temperature") std::cout << " C";
            if (param == "air_humidity") std::cout << " %";
            if (param == "soil_moisture") std::cout << " %";
            std::cout << std::endl;
        }

        // Расчёт управляющих команд (делегирование ClimateManager)
        auto commands = m_climateManager->calculateCommands(currentValues);

        // Применение команд к устройствам и модели
        applyCommands(commands);

        //  Обновление модели (естественные процессы + эффекты устройств)
        if (m_model) {
            m_model->update();
            m_model->printParameters();
        }

        // Пауза перед следующим циклом
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

/**
 * @brief Остановка цикла управления
 *
 * Устанавливает флаг m_is_running = false, что приводит к завершению
 * главного цикла в методе start(). Затем дожидается остановки всех
 * фоновых таймеров (расписаний).
 */
void SimulationEngine::stop() {
    m_is_running = false;  // Сигнал для выхода из главного цикла

    // Остановка всех фоновых таймеров
    for (auto& pair : m_schedules) {
        if (pair.second) {
            pair.second->stop();
        }
    }

    std::cout << "[INFO]: SimulationEngine stopped" << std::endl;
}