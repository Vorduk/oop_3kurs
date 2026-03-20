#pragma once

// Интерфейсы устройств

// Интерфейс для устройств
class IDevice {
public:
    virtual ~IDevice() = default;

    // Общие методы для всех устройств
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool isOn() const = 0;
};

// Интерфейс для устройств с регулировкой мощности
class IAdjustableDevice {
public:
    virtual ~IAdjustableDevice() = default;
    virtual void setPower(int level) = 0;  // 0-100%
    virtual int getPower() const = 0;
};

// Интерфейс для устройств с фиксированными режимами
class IModeSelectableDevice {
public:
    virtual ~IModeSelectableDevice() = default;
    virtual void setMode(int mode) = 0;
    virtual int getMode() const = 0;
};

