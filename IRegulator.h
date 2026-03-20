#pragma once
#include <string>

// Интерфейс для алгоритмов регулирования (паттерн Delegation)
class IRegulator {
public:
    virtual ~IRegulator() = default;

    // Рассчитать управляющий сигнал (0-100) на основе текущего и целевого значения
    virtual double calculate(double current, double target) = 0;

    // Сбросить внутреннее состояние (для PID)
    virtual void reset() {}

    // Получить название регулятора
    virtual std::string getName() const = 0;
};
