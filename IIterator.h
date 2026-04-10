#pragma once

class IIterator {
public:
    virtual ~IIterator() = default;
    virtual bool hasNext() const = 0;
    virtual void* next() = 0;
};