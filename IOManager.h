#pragma once
#include "IIOManager.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Реализация менеджера ввода/вывода
 *
 * Управляет всеми датчиками и исполнительными устройствами в системе.
 * Хранит объекты в ассоциативных контейнерах для быстрого доступа по ID.
 *
 * Датчики хранятся в map<int, shared_ptr<ISensor>> для доступа по ID
 * Устройства хранятся в двух контейнерах: по ID и по типу
 * Поддерживает команды трёх типов: On/Off, мощность, режим
 *
 * @note Использует умные указатели для автоматического управления памятью
 */
class IOManager : public IIOManager {
public:
    // Управление датчиками

    /**
     * @brief Добавить датчик в систему
     * @param sensor Умный указатель на датчик
     *
     * Датчик сохраняется в контейнере с ключом sensor->getId().
     * При добавлении выводится информационное сообщение.
     */
    void addSensor(std::shared_ptr<ISensor> sensor) override;

    /**
     * @brief Удалить датчик из системы по идентификатору
     * @param sensorId Уникальный идентификатор датчика
     *
     * Датчик удаляется из контейнера. Память освобождается автоматически,
     * так как используется shared_ptr.
     */
    void removeSensor(int sensorId) override;

    /**
     * @brief Получить датчик по идентификатору
     * @param sensorId Уникальный идентификатор датчика
     * @return Умный указатель на датчик или nullptr, если не найден
     */
    std::shared_ptr<ISensor> getSensor(int sensorId) override;

    /**
     * @brief Считать показания со всех датчиков
     * @return Ассоциативный массив "идентификатор датчика -> текущее значение"
     *
     * Проходит по всем датчикам в контейнере и вызывает getValue() у каждого.
     * Если датчик по какой-то причине недоступен, его значение не включается.
     */
    std::map<int, double> readAllSensors() override;

    // Управление устройствами

    /**
     * @brief Добавить исполнительное устройство в систему
     * @param device Умный указатель на устройство
     *
     * Устройство сохраняется в двух контейнерах
     * m_devices_by_id: для доступа по ID
     * m_devices_by_type: для получения всех устройств определённого типа
     */
    void addDevice(std::shared_ptr<IDevice> device) override;

    /**
     * @brief Удалить устройство из системы по идентификатору
     * @param deviceId Уникальный идентификатор устройства
     *
     * Устройство удаляется из обоих контейнеров.
     */
    void removeDevice(int deviceId) override;

    /**
     * @brief Получить устройство по идентификатору
     * @param deviceId Уникальный идентификатор устройства
     * @return Умный указатель на устройство или nullptr, если не найдено
     */
    std::shared_ptr<IDevice> getDevice(int deviceId) override;

    // Команды управления устройствами

    /**
     * @brief Отправить команду включения/выключения устройству
     * @param deviceId Идентификатор устройства
     * @param turnOn true — включить, false — выключить
     *
     * Работает с любым устройством, реализующим IDevice.
     * Если устройство уже находится в требуемом состоянии, команда игнорируется.
     */
    void sendOnOffCommand(int deviceId, bool turnOn) override;

    /**
     * @brief Отправить команду установки мощности устройству
     * @param deviceId Идентификатор устройства
     * @param powerLevel Уровень мощности (0-100%)
     *
     * Требует, чтобы устройство реализовывало IAdjustableDevice.
     * Если устройство не поддерживает регулировку мощности, команда игнорируется.
     * Если powerLevel > 0, устройство автоматически включается.
     */
    void sendPowerCommand(int deviceId, int powerLevel) override;

    /**
     * @brief Отправить команду установки режима устройству
     * @param deviceId Идентификатор устройства
     * @param mode Код режима (определяется конкретным устройством)
     *
     * Требует, чтобы устройство реализовывало IModeSelectableDevice.
     * Если устройство не поддерживает выбор режима, команда игнорируется.
     */
    void sendModeCommand(int deviceId, int mode) override;

    /**
     * @brief Получить список идентификаторов устройств по типу
     * @param type Тип устройства ("heater", "conditioner", "ventilation" и т.д.)
     * @return Вектор идентификаторов устройств указанного типа
     *
     * Используется для массовой отправки команд всем устройствам одного типа.
     */
    std::vector<int> getDeviceIdsByType(const std::string& type);

private:
    std::map<int, std::shared_ptr<ISensor>> m_sensors_by_id;           ///< Датчики по ID
    std::map<int, std::shared_ptr<IDevice>> m_devices_by_id;           ///< Устройства по ID
    std::map<std::string, std::vector<int>> m_devices_by_type;         ///< Устройства по типу
};