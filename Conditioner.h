#pragma once
#include "BaseDevice.h"
#include "IDeviceDriver.h"
#include <memory>

/**
 * @brief Кондиционер для понижения температуры в теплице
 *
 * Поддерживает плавную регулировку мощности (0-100%).
 * Реализует интерфейсы IDevice и IAdjustableDevice.
 */
class Conditioner : public BaseDevice, public IAdjustableDevice {
private:
    std::shared_ptr<IDeviceDriver> m_driver;
    int m_last_power = 0;

public:
    Conditioner(std::shared_ptr<IDeviceDriver> driver);
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

