#include "PIDRegulator.h"

PidRegulator::PidRegulator(double kp, double ki, double kd, double dt)
    : m_kp(kp), m_ki(ki), m_kd(kd), m_dt(dt), m_integral(0), m_prevError(0) {
}

double PidRegulator::calculate(double current, double target)
{
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

void PidRegulator::reset()
{
    m_integral = 0;
    m_prevError = 0;
}

std::string PidRegulator::getName() const
{
    return "PidRegulator";
}

void PidRegulator::setGains(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    reset();
}
