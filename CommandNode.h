// CommandNode.h
#pragma once
#include "Command.h"

class CommandNode {
private:
    Command m_command;
    CommandNode* m_next;

public:
    CommandNode(const Command& cmd);
    ~CommandNode();

    Command& getCommand();
    const Command& getCommand() const;
    CommandNode* getNext() const;
    void setNext(CommandNode* next);

    void push(const Command& cmd);
};