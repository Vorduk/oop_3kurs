#include "DeviceLoggerDecorator.h"

DeviceLoggerDecorator::DeviceLoggerDecorator(std::shared_ptr<IDevice> device)
    : DeviceDecorator(device) {
}

void DeviceLoggerDecorator::turnOn() {
    std::cout << "[DeviceLoggerDecorator]: Turning ON " << getType()
        << " (ID:" << getId() << ")" << std::endl;
    DeviceDecorator::turnOn();
}

void DeviceLoggerDecorator::turnOff() {
    std::cout << "[DeviceLoggerDecorator]: Turning OFF " << getType()
        << " (ID:" << getId() << ")" << std::endl;
    DeviceDecorator::turnOff();
}

void DeviceLoggerDecorator::setPower(int level) {
    std::cout << "[DeviceLoggerDecorator]: Setting power of " << getType()
        << " (ID:" << getId() << ") to " << level << "%" << std::endl;
    DeviceDecorator::setPower(level);
}

void DeviceLoggerDecorator::setMode(int mode) {
    std::cout << "[DeviceLoggerDecorator]: Setting mode of " << getType()
        << " (ID:" << getId() << ") to " << mode << std::endl;
    DeviceDecorator::setMode(mode);
}