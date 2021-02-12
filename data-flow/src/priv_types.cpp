#include "priv_types.hpp"

namespace dt::df
{
    RefCon::~RefCon()
    {
        connection.disconnect();
    }

} // namespace dt::df
