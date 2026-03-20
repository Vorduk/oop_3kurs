#pragma once
#include "IRegulator.h"
#include <cmath>

// Простой On/Off регулятор с гистерезисом (паттерн Delegation)
class OnOffRegulator : public IRegulator {
private:
    double m_hysteresis;
    bool m_lastState;

public:
    OnOffRegulator(double hysteresis = 1.0)
        : m_hysteresis(hysteresis), m_lastState(false) {
    }

    double calculate(double current, double target) override {
        if (current < target - m_hysteresis) {
            m_lastState = true;
            return 100.0;  // Включить на полную
        }
        else if (current > target + m_hysteresis) {
            m_lastState = false;
            return 0.0;    // Выключить
        }
        // В гистерезисе - сохраняем предыдущее состояние
        return m_lastState ? 100.0 : 0.0;
    }

    std::string getName() const override {
        return "OnOffRegulator";
    }

    void setHysteresis(double hysteresis) {
        m_hysteresis = hysteresis;
    }
};