#include <iostream>
#include <memory>
#include "ObjectPool.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"

int main() {
    // Получение глобального пула объектов
    ObjectPool* pool = ObjectPool::Instance();

    // Инициализация пула двумя нагревателями
    pool->initialize(2, []() -> std::shared_ptr<IDevice> {
        return std::make_shared<Heater>();
        });

    std::cout << "\nGreenhouse 1 requests devices\n";
    auto heater1 = pool->acquireDevice();
    if (heater1) {
        std::cout << "Greenhouse 1 got device: " << heater1->getType() << " (ID:" << heater1->getId() << ")\n";
        heater1->turnOn();
    }

    auto heater2 = pool->acquireDevice();
    if (heater2) {
        std::cout << "Greenhouse 1 got device: " << heater2->getType() << " (ID:" << heater2->getId() << ")\n";
        heater2->turnOn();
    }

    std::cout << "\nGreenhouse 2 requests a device\n";
    auto missing_device = pool->acquireDevice();
    if (!missing_device) {
        std::cout << "Greenhouse 2 failed to get a device, none available\n";
    }

    // Возврат одного устройства из первой теплицы
    std::cout << "\nGreenhouse 1 returns one heater back to pool\n";
    pool->releaseDevice(heater1);

    std::cout << "\nGreenhouse 2 tries again after return\n";
    auto recycled_device = pool->acquireDevice();
    if (recycled_device) {
        std::cout << "Greenhouse 2 got device: " << recycled_device->getType() << " (ID:" << recycled_device->getId() << ")\n";
        recycled_device->turnOn();
    }

    // Возврат всех устройств в пул
    std::cout << "\nReturning all devices to pool\n";
    pool->releaseDevice(heater2);
    pool->releaseDevice(recycled_device);

    return 0;
}