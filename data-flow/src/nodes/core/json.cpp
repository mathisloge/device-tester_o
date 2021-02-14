#include "json.hpp"

using json = nlohmann::json;
namespace dt::df
{

    void to_json(json &j, const BaseNode &p)
    {
        p.to_json(j);
    }
    
} // namespace dt::df
