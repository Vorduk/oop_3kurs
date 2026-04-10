#pragma once
#include "IIterator.h"
#include "CommandNode.h"
#include <iostream>

class CommandHistory;

class CommandIterator : public IIterator {
private:
    CommandNode* m_current;
    CommandHistory* m_history;

public:
    CommandIterator(CommandNode* start, CommandHistory* history = nullptr);

    bool hasNext() const override;

    void* next() override;
};
