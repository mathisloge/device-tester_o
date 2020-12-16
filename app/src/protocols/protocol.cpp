#include "protocol.hpp"

Protocol::Protocol()
    : state_{State::activated}
{
}

void Protocol::setState(State state)
{
    state_ = state;
}

bool Protocol::isActive() const
{
    return state_ == State::activated;
}