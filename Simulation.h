#pragma once
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

class Simulation {
public:
    Simulation(double initial_temperature, double initial_air_humidity, double initial_soil_moisture);
    void start();
    void stop();

private:
    // Текущие значения параметров
    double m_temperature; // Температура, градусы цельсия
    double m_air_humidity; // Относительная влажность воздуха, %
    double m_soil_moisture; // Относительная влажность почвы, %
    
    bool m_is_running;

    // Генератор случайных чисел
    std::mt19937 m_rng; // Генератор
    std::uniform_real_distribution<> m_noise; // Распределение [0, 1)

    void update();
    void printParameters();
};
