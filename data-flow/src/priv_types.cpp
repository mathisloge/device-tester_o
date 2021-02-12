#include "priv_types.hpp"

namespace dt::df
{
    EdgeInfo::~EdgeInfo()
    {
        connection.disconnect();
    }
} // namespace dt::df
