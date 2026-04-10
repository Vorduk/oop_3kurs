#include "DevicePowerLimitDecorator.h"

DevicePowerLimitDecorator::DevicePowerLimitDecorator(std::shared_ptr<IDevice> device, int max_power)
    : DeviceDecorator(device), m_max_power(max_power) {
    if (m_max_power < 0) m_max_power = 0;
    if (m_max_power > 100) m_max_power = 100;
}

void DevicePowerLimitDecorator::setPower(int level) {
    if (level > m_max_power) {
        std::cout << "[DevicePowerLimitDecorator]$ Device " << getType()
            << " (ID:" << getId() << ") reducing power from "
            << level << "% to " << m_max_power << "%" << std::endl;
        level = m_max_power;
    }
    DeviceDecorator::setPower(level);
}