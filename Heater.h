#pragma once
#include "BaseDevice.h"

/**
 * @brief Нагреватель для повышения температуры в теплице
 *
 * Поддерживает плавную регулировку мощности (0-100%).
 * Реализует интерфейсы IDevice и IAdjustableDevice.
 */
class Heater : public BaseDevice, public IAdjustableDevice {
public:
    Heater();
    ~Heater() override = default;

    // Методы IDevice

    /** @brief Вкл нагреватель */
    void turnOn() override;

    /** @brief Выкл нагреватель */
    void turnOff() override;

    /** @brief Включён ли нагреватель */
    bool isOn() const override;


    // Методы IAdjustableDevice

    /** @brief Изменить мощность нагревателя */
    void setPower(int level) override;

    /** @brief Получить текущую мощность нагревателя */
    int getPower() const override;

private:
    bool m_isOn;        ///< Состояние устройства (включен/выключен)
    int m_powerLevel;   ///< Уровень мощности в процентах (0-100)
};

