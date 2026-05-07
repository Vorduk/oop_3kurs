// ObjectPool.cpp
#include "ObjectPool.h"

ObjectPool* ObjectPool::instance = nullptr;

ObjectPool* ObjectPool::Instance() {
    if (instance == nullptr) {
        instance = new ObjectPool();
    }
    return instance;
}

void ObjectPool::initialize(int count, std::function<std::shared_ptr<IDevice>()> factory) {
    creation_factory = factory;
    max_capacity = count;
    total_created = 0;
    available_devices.clear();

    // Предварительное создание объектов для пула
    for (int i = 0; i < count; ++i) {
        auto device = creation_factory();
        available_devices.push_back(device);
        ++total_created;
    }
    std::cout << "[ObjectPool] Pool initialized with " << count << " devices\n";
}

std::shared_ptr<IDevice> ObjectPool::acquireDevice() {
    if (!available_devices.empty()) {
        auto device = available_devices.back();
        available_devices.pop_back();
        std::cout << "[ObjectPool] Device acquired from pool, available: " << available_devices.size() << "\n";
        return device;
    }
    std::cout << "[ObjectPool] No free devices in pool\n";
    return nullptr; // Объекты закончились
}

void ObjectPool::releaseDevice(std::shared_ptr<IDevice> device) {
    if (device) {
        available_devices.push_back(device);
        std::cout << "[ObjectPool] Device returned to pool, available: " << available_devices.size() << "\n";
    }
}