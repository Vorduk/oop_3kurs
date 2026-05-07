#include "DeviceInfoVisitor.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"
#include "OldHeaterAdapter.h"
#include <iostream>

void DeviceInfoVisitor::visit(Heater* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    std::cout << "  Power: " << device->getPower() << "%\n";
}

void DeviceInfoVisitor::visit(Conditioner* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    std::cout << "  Power: " << device->getPower() << "%\n";
}

void DeviceInfoVisitor::visit(AirHumidifier* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    int mode = device->getMode();
    const char* modeStr = "unknown";
    if (mode == AirHumidifier::MODE_LOW) modeStr = "low";
    else if (mode == AirHumidifier::MODE_MEDIUM) modeStr = "medium";
    else if (mode == AirHumidifier::MODE_HIGH) modeStr = "high";
    std::cout << "  Mode: " << modeStr << " (" << mode << ")\n";
}

void DeviceInfoVisitor::visit(Irrigation* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    int mode = device->getMode();
    const char* modeStr = (mode == Irrigation::MODE_DRIP) ? "drip" : "sprinkle";
    std::cout << "  Mode: " << modeStr << " (" << mode << ")\n";
}

void DeviceInfoVisitor::visit(Ventilation* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    int mode = device->getMode();
    const char* modeStr = "unknown";
    if (mode == Ventilation::MODE_LOW) modeStr = "low";
    else if (mode == Ventilation::MODE_MEDIUM) modeStr = "medium";
    else if (mode == Ventilation::MODE_HIGH) modeStr = "high";
    std::cout << "  Mode: " << modeStr << " (" << mode << ")\n";
}

void DeviceInfoVisitor::visit(Lamp* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    std::cout << "  Power: " << device->getPower() << "%\n";
    int mode = device->getMode();
    const char* modeStr = "unknown";
    if (mode == Lamp::MODE_WHITE) modeStr = "white";
    else if (mode == Lamp::MODE_WARM) modeStr = "warm";
    else if (mode == Lamp::MODE_UF) modeStr = "uf";
    std::cout << "  Mode: " << modeStr << " (" << mode << ")\n";
}

void DeviceInfoVisitor::visit(OldHeaterAdapter* device) {
    std::cout << "[Visitor] Device: " << device->getType()
        << " (ID=" << device->getId() << ")\n";
    std::cout << "  ON: " << (device->isOn() ? "yes" : "no") << "\n";
    std::cout << "  Power: " << device->getPower() << "%\n";
}