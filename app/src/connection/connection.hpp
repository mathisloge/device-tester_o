#pragma once

class Connection
{
public:
    virtual void open() = 0;
    virtual void close() = 0;
};