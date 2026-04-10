#pragma once
#include "IAggregate.h"
#include "CommandNode.h"
#include "CommandIterator.h"

class CommandHistory : public IAggregate {
private:
    CommandNode* m_root;

public:
    CommandHistory();

    ~CommandHistory();

    void push(const Command& cmd);

    IIterator* createIterator() override;
};