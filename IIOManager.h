#pragma once
#include "ISensor.h"
#include "IDevice.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

/**
 * @brief Интерфейс менеджера ввода/вывода
 *
 * Обеспечивает унифицированный доступ к датчикам и исполнительным устройствам.
 * Инкапсулирует операции добавления, удаления и управления оборудованием.
 */
class IIOManager {
public:
    virtual ~IIOManager() = default;

    // Управление датчиками

    /**
     * @brief Добавить датчик в систему
     * @param sensor Умный указатель на датчик
     */
    virtual void addSensor(std::shared_ptr<ISensor> sensor) = 0;

    /**
     * @brief Удалить датчик из системы по идентификатору
     * @param sensorId Уникальный идентификатор датчика
     * 
     * Память освобождается автоматически (shared_ptr).
     */
    virtual void removeSensor(int sensorId) = 0;

    /**
     * @brief Получить датчик по идентификатору
     * @param sensorId Уникальный идентификатор датчика
     * @return Умный указатель на датчик или nullptr, если не найден
     */
    virtual std::shared_ptr<ISensor> getSensor(int sensorId) = 0;

    /**
     * @brief Считать показания со всех датчиков
     * @return map идентификатор датчика -> текущее значение
     */
    virtual std::map<int, double> readAllSensors() = 0;

    // Управление устройствами

    /**
     * @brief Добавить исполнительное устройство в систему
     * @param device Умный указатель на устройство (разделяемое владение)
     */
    virtual void addDevice(std::shared_ptr<IDevice> device) = 0;

    /**
     * @brief Удалить устройство из системы по идентификатору
     * @param deviceId Уникальный идентификатор устройства
     *
     * Память освобождается автоматически (shared_ptr).
     */
    virtual void removeDevice(int deviceId) = 0;

    /**
     * @brief Получить устройство по идентификатору
     * @param deviceId Уникальный идентификатор устройства
     * @return Умный указатель на устройство или nullptr, если не найдено
     */
    virtual std::shared_ptr<IDevice> getDevice(int deviceId) = 0;

    // Команды управления устройствами

    /**
     * @brief Отправить команду включения/выключения устройству
     * @param deviceId Идентификатор устройства
     * @param turnOn true - включить, false - выключить
     *
     * Работает с любым устройством, реализующим IDevice.
     */
    virtual void sendOnOffCommand(int deviceId, bool turnOn) = 0;

    /**
     * @brief Отправить команду установки мощности устройству
     * @param deviceId Идентификатор устройства
     * @param powerLevel Уровень мощности (0-100%)
     *
     * Требует, чтобы устройство реализовывало IAdjustableDevice.
     * Если устройство не поддерживает регулировку мощности, команда игнорируется.
     */
    virtual void sendPowerCommand(int deviceId, int powerLevel) = 0;

    /**
     * @brief Отправить команду установки режима устройству
     * @param deviceId Идентификатор устройства
     * @param mode Код режима (определяется конкретным устройством)
     *
     * Требует, чтобы устройство реализовывало IModeSelectableDevice.
     * Если устройство не поддерживает выбор режима, команда игнорируется.
     */
    virtual void sendModeCommand(int deviceId, int mode) = 0;
};