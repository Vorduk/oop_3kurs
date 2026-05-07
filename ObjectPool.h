// ObjectPool.h
#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include "IDevice.h"

class ObjectPool {
public:
    // Получение единственного экземпляра пула (Singleton)
    static ObjectPool* Instance();

    // Заполнение пула объектами заданного типа (фабричный метод)
    void initialize(int count, std::function<std::shared_ptr<IDevice>()> factory);

    // Извлечение доступного объекта из пула
    std::shared_ptr<IDevice> acquireDevice();

    // Возврат объекта в пул
    void releaseDevice(std::shared_ptr<IDevice> device);

private:
    ObjectPool() = default;
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    static ObjectPool* instance;

    std::vector<std::shared_ptr<IDevice>> available_devices; // Свободные объекты
    int max_capacity = 0;                                    // Максимальное число объектов
    int total_created = 0;                                   // Всего создано объектов
    std::function<std::shared_ptr<IDevice>()> creation_factory; // Фабрика объектов
};