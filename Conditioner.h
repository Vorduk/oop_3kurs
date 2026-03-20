#pragma once
#include "BaseDevice.h"

/**
 * @brief Кондиционер для понижения температуры в теплице
 *
 * Поддерживает плавную регулировку мощности (0-100%).
 * Реализует интерфейсы IDevice и IAdjustableDevice.
 */
class Conditioner : public BaseDevice, public IAdjustableDevice {
private:
    bool m_isOn;        ///< Состояние устройства (включен/выключен)
    int m_powerLevel;   ///< Уровень мощности в процентах (0-100)

public:
    Conditioner();
    ~Conditioner() override = default;

    // Методы IDevice

    /** @brief Вкл кондиционер */
    void turnOn() override;

    /** @brief Выкл кондиционер */
    void turnOff() override;

    /** @brief Включён ли кондиционер */
    bool isOn() const override;


    // Методы IAdjustableDevice

    /** @brief Изменить мощность кондиционера */
    void setPower(int level) override;

    /** @brief Получить текущую мощность кондиционера */
    int getPower() const override;
};

