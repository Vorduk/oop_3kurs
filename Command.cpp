#include "Command.h"

Command::Command(const std::string& device_type, int power)
    : m_timestamp(std::time(nullptr))
    , m_device_type(device_type)
    , m_power(power) {
}

std::string Command::getDescription() const
{
    return m_device_type + " -> " + std::to_string(m_power) + "%";
}

std::string Command::getDeviceType() const
{
    return m_device_type;
}

int Command::getPower() const
{
    return m_power;
}

std::time_t Command::getTimestamp() const
{
    return m_timestamp;
}
