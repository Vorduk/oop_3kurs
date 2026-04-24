#pragma once
#include "SystemMemento.h"
#include <stack>

// Класс-клиент, управляющий историей снимков.
class HistoryManager {
public:
    HistoryManager();

    // Сохраняет снимок в стек.
    void push(const SystemMemento& memento);

    // Восстанавливает последний сохранённый снимок, удаляя его из стека.
    // Возвращает true, если снимок был доступен.
    bool pop(SystemMemento& outMemento);

    // Возвращает количество сохранённых снимков.
    int size() const;

private:
    std::stack<SystemMemento> m_history;
};