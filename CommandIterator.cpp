#include "CommandIterator.h"

CommandIterator::CommandIterator(CommandNode* start, CommandHistory* history)
    : m_current(start)
    , m_history(history) {
}

bool CommandIterator::hasNext() const
{
    return m_current != nullptr;
}

void* CommandIterator::next()
{
    if (!m_current) return nullptr;
    void* result = &(m_current->getCommand());
    m_current = m_current->getNext();
    return result;
}
