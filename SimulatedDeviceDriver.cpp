#include "SimulatedDeviceDriver.h"
#include <iostream>

void SimulatedDeviceDriver::hardwarePowerOn()
{
    m_isOn = true;
    std::cout << "[SimulatedDeviceDriver] " << "simulated device" << " -> POWER ON" << std::endl;
}

void SimulatedDeviceDriver::hardwarePowerOff()
{
    m_isOn = false;
    m_power = 0;
    std::cout << "[SimulatedDeviceDriver] " << "simulated device" << " -> POWER OFF" << std::endl;
}

void SimulatedDeviceDriver::hardwareSetPower(int level)
{
    if (m_isOn) { 
        m_power = level; 
        std::cout << "[SimulatedDeviceDriver] " << "simulated device" << " -> SET POWER " << level << "%" << std::endl;
    }
}

bool SimulatedDeviceDriver::isHardwareOn() const
{
    return m_isOn;
}

