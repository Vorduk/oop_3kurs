#pragma once
#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <iostream>

class TimerSchedule {
private:
    std::atomic<bool> m_running;
    std::thread m_thread;
    std::function<void()> m_callback;
    std::chrono::seconds m_interval;

public:
    TimerSchedule() : m_running(false), m_interval(0) {}  // Инициализируем m_interval

    ~TimerSchedule() {
        stop();
    }

    void start(std::chrono::seconds interval, std::function<void()> callback) {
        if (m_running) return;

        m_interval = interval;
        m_callback = callback;
        m_running = true;

        m_thread = std::thread([this]() {
            while (m_running) {
                std::this_thread::sleep_for(m_interval);
                if (m_running && m_callback) {
                    m_callback();
                }
            }
            });
    }

    void stop() {
        m_running = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
};