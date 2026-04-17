#pragma once
#include "BaseDevice.h"
#include "IDeviceDriver.h"
#include "memory"

/**
 * @brief Нагреватель для повышения температуры в теплице
 *
 * Поддерживает плавную регулировку мощности (0-100%).
 * Реализует интерфейсы IDevice и IAdjustableDevice.
 */
class Heater : public BaseDevice, public IAdjustableDevice {
public:
    Heater(std::shared_ptr<IDeviceDriver> driver);
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
    std::shared_ptr<IDeviceDriver> m_driver;
    int m_last_power = 0;
};

