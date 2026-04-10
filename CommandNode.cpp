#include "CommandNode.h"

CommandNode::CommandNode(const Command& cmd)
    : m_command(cmd)
    , m_next(nullptr) {
}

CommandNode::~CommandNode()
{
    delete m_next;
}

Command& CommandNode::getCommand()
{
    return m_command;
}

const Command& CommandNode::getCommand() const
{
    return m_command;
}

CommandNode* CommandNode::getNext() const
{
    return m_next;
}

void CommandNode::setNext(CommandNode* next)
{
    m_next = next;
}

void CommandNode::push(const Command& cmd)
{
    if (m_next)
        m_next->push(cmd);
    else
        m_next = new CommandNode(cmd);
}
