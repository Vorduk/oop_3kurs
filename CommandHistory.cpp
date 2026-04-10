#include "CommandHistory.h"

CommandHistory::CommandHistory()
	: m_root(nullptr) {
}

CommandHistory::~CommandHistory()
{
    delete m_root;
}

void CommandHistory::push(const Command& cmd)
{
    if (!m_root)
        m_root = new CommandNode(cmd);
    else
        m_root->push(cmd);
}

IIterator* CommandHistory::createIterator()
{
    return new CommandIterator(m_root, this);
}
