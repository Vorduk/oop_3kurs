#pragma once

/**
 * @brief Интерфейс низкоуровневого драйвера устройства (Реализация для Bridge)
 */
class IDeviceDriver {
public:
    virtual ~IDeviceDriver() = default;

    virtual void hardwarePowerOn() = 0;
    virtual void hardwarePowerOff() = 0;
    virtual void hardwareSetPower(int level) = 0;
    virtual bool isHardwareOn() const = 0;
};