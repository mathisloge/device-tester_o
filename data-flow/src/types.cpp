#include "data-flow/types.hpp"

namespace dt::df
{
    EdgeInfo::~EdgeInfo()
    {
        connection.disconnect();
    }
} // namespace dt::df
