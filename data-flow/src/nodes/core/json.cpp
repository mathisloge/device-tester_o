#include "json.hpp"

using json = nlohmann::json;
namespace dt::df
{

    void to_json(json &j, const BaseNode &p)
    {
        p.to_json(j);
    }

    void from_json(const json &j, BaseNode &p)
    {
    }
} // namespace dt::df
