#pragma once
#include <string>
#include <ctime>

class Command {
private:
    std::time_t m_timestamp;
    std::string m_device_type;
    int m_power;

public:
    Command(const std::string& deviceType, int power);

    std::string getDescription() const;

    std::string getDeviceType() const;
    int getPower() const;
    std::time_t getTimestamp() const;
};