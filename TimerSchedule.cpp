#include "TimerSchedule.h"

TimerSchedule::TimerSchedule() 
	: m_running(false), m_interval(0) {
}

void TimerSchedule::start(std::chrono::seconds interval, std::function<void()> callback)
{
    if (m_running) return;

    m_interval = interval;
    m_callback = callback;
    m_running = true;

    // Создание нового потока выполнения.Лямбда - функция захватывает this, чтобы иметь доступ к полям класса.
    // Поток засыпает на заданный интервал с помощью sleep_for().
    // После пробуждения проверяет, должен ли таймер продолжать работу(m_running) и существует ли callback - функция.
    // Если условия выполнены, вызывает m_callback().
    // Цикл повторяется до тех пор, пока m_running = true.
    m_thread = std::thread([this]() { 
        while (m_running) {
            std::this_thread::sleep_for(m_interval);
            if (m_running && m_callback) {
                m_callback();
            }
        }
        });
}

void TimerSchedule::stop()
{
    // joinable() проверяет, что поток был запущен и ещё не завершён.
    // join() блокирует текущий поток до полного завершения рабочего потока.
    // Это гарантирует, что после вызова stop() рабочий поток больше не выполняется.
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}
