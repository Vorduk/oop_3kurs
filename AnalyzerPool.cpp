#include "AnalyzerPool.h"
#include <iostream>

AnalyzerPool& AnalyzerPool::getInstance() {
    static AnalyzerPool instance(3);  // Singleton, размер пула = 3
    return instance;
}

AnalyzerPool::AnalyzerPool(size_t poolSize)
    : m_poolSize(poolSize)
{
    std::cout << "[AnalyzerPool] Initializing pool with " << m_poolSize << " analyzers..." << std::endl;
    for (size_t i = 0; i < m_poolSize; ++i) {
        m_allObjects.push_back(std::make_unique<ClimateAnalyzer>());
        m_freeObjects.push_back(m_allObjects.back().get());
    }
    std::cout << "[AnalyzerPool] Pool ready. Available: " << availableCount() << std::endl;
}

ClimateAnalyzer* AnalyzerPool::acquire() {
    if (m_freeObjects.empty()) {
        std::cout << "[AnalyzerPool] No free analyzers available!" << std::endl;
        return nullptr;
    }
    ClimateAnalyzer* analyzer = m_freeObjects.back();
    m_freeObjects.pop_back();
    std::cout << "[AnalyzerPool] Acquired analyzer #" << analyzer->getId()
        << ". Available: " << m_freeObjects.size() << std::endl;
    return analyzer;
}

void AnalyzerPool::release(ClimateAnalyzer* analyzer) {
    if (!analyzer) return;
    analyzer->reset();
    m_freeObjects.push_back(analyzer);
    std::cout << "[AnalyzerPool] Released analyzer #" << analyzer->getId()
        << ". Available: " << m_freeObjects.size() << std::endl;
}

size_t AnalyzerPool::availableCount() const {
    return m_freeObjects.size();
}