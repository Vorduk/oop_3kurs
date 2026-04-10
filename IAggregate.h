#pragma once
#include "IIterator.h"

class IAggregate {
public:
    virtual ~IAggregate() = default;
    virtual IIterator* createIterator() = 0;
};