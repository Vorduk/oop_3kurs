#include "HistoryManager.h"
#include <iostream>

HistoryManager::HistoryManager() {
}

void HistoryManager::push(const SystemMemento& memento) {
    m_history.push(memento);
    std::cout << "[HistoryManager] Snapshot saved. Total snapshots: "
        << m_history.size() << std::endl;
}

bool HistoryManager::pop(SystemMemento& outMemento) {
    if (m_history.empty()) {
        std::cout << "[HistoryManager] No snapshots to restore." << std::endl;
        return false;
    }
    outMemento = m_history.top();
    m_history.pop();
    std::cout << "[HistoryManager] Last snapshot restored. Remaining: "
        << m_history.size() << std::endl;
    return true;
}

int HistoryManager::size() const {
    return static_cast<int>(m_history.size());
}