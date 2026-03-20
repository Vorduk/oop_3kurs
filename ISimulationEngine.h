#pragma once

// Управляет циклом обновления
class ISimulationEngine {
public:
    virtual ~ISimulationEngine() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
};