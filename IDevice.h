#pragma once
#include <string>

class IDeviceVisitor;

/**
 * Базовый интерфейс для всех исполнительных устройств
 */
class IDevice {
public:
    virtual ~IDevice() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool isOn() const = 0;
    virtual std::string getType() const = 0;
    virtual int getId() const = 0;

    // Метод для принятия посетителя (паттерн Visitor)
    virtual void accept(IDeviceVisitor* visitor) = 0;
};

class IAdjustableDevice {
public:
    virtual ~IAdjustableDevice() = default;
    virtual void setPower(int level) = 0;
    virtual int getPower() const = 0;
};

class IModeSelectableDevice {
public:
    virtual ~IModeSelectableDevice() = default;
    virtual void setMode(int mode) = 0;
    virtual int getMode() const = 0;
};