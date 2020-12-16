#pragma once
#include <vector>
#include "protocol_types.hpp"
class Protocol
{
public:
    enum class State
    {
        activated,
        deactivated
    };

public:
    Protocol();
    void setState(State state);
    bool isActive() const;
    virtual const char *name() const = 0;
    /**
     * @brief 
     * 
     * @param begin 
     * @param end
     * @return std::pair<Data::const_iterator, Data::const_iterator>  return pair<param begin, param begin> if isActive is false
     */
    virtual std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) = 0;

protected:
    State state_;
};