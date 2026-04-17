#include "ConsoleDeviceDriver.h"

ConsoleDeviceDriver::ConsoleDeviceDriver(const std::string& name)
    : m_device_name(name) {
}

void ConsoleDeviceDriver::hardwarePowerOn()
{
    m_isOn = true;
    std::cout << "[ConsoleDeviceDriver] " << m_device_name << " -> POWER ON" << std::endl;
}

void ConsoleDeviceDriver::hardwarePowerOff()
{
    m_isOn = false;
    m_power = 0;
    std::cout << "[ConsoleDeviceDriver] " << m_device_name << " -> POWER OFF" << std::endl;
}

void ConsoleDeviceDriver::hardwareSetPower(int level)
{
    if (m_isOn) {
        m_power = level;
        std::cout << "[ConsoleDeviceDriver] " << m_device_name << " -> SET POWER " << level << "%" << std::endl;
    }
}

bool ConsoleDeviceDriver::isHardwareOn() const
{
    return m_isOn;
}
