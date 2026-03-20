#pragma once
#include "IRegulator.h"
#include <cmath>

// ПИД-регулятор (паттерн Delegation)
class PidRegulator : public IRegulator {
private:
    double m_kp;  // Пропорциональный коэффициент
    double m_ki;  // Интегральный коэффициент
    double m_kd;  // Дифференциальный коэффициент

    double m_integral;
    double m_prevError;
    double m_dt;

public:
    PidRegulator(double kp = 2.0, double ki = 0.1, double kd = 0.5, double dt = 1.0)
        : m_kp(kp), m_ki(ki), m_kd(kd), m_dt(dt), m_integral(0), m_prevError(0) {
    }

    double calculate(double current, double target) override {
        double error = target - current;

        // Пропорциональная составляющая
        double pTerm = m_kp * error;

        // Интегральная составляющая (с антивинд-апом)
        m_integral += error * m_dt;
        if (m_integral > 100 / m_ki) m_integral = 100 / m_ki;
        if (m_integral < -100 / m_ki) m_integral = -100 / m_ki;
        double iTerm = m_ki * m_integral;

        // Дифференциальная составляющая
        double derivative = (error - m_prevError) / m_dt;
        double dTerm = m_kd * derivative;

        m_prevError = error;

        double output = pTerm + iTerm + dTerm;

        // Ограничиваем выход 0-100
        if (output > 100) output = 100;
        if (output < 0) output = 0;

        return output;
    }

    void reset() override {
        m_integral = 0;
        m_prevError = 0;
    }

    std::string getName() const override {
        return "PidRegulator";
    }

    void setGains(double kp, double ki, double kd) {
        m_kp = kp;
        m_ki = ki;
        m_kd = kd;
        reset();
    }
};