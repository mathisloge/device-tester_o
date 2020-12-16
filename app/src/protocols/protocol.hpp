#pragma once
#include <vector>
class Protocol
{
public:
    using Data = std::vector<uint8_t>;
    enum class State
    {
        activated,
        deactivated
    };

public:
    Protocol();
    virtual const char *name() const = 0;
    void setState(State state);
    bool isActive() const;

    /**
     * @brief 
     * @attention have to return false if isActive is false.
     * @param begin 
     * @param end 
     * @return true 
     * @return false no message is present or isActive return false
     */
    virtual bool containsMessage(Data::const_iterator begin, Data::const_iterator end) const = 0;
    virtual std::pair<Data::const_iterator, Data::const_iterator> consumeOneMessage(Data::const_iterator begin, Data::const_iterator end) = 0;

protected:
    State state_;
};