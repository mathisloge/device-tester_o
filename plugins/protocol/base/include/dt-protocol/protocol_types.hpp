#pragma once
#include <cstdint>
#include <vector>

namespace protocol
{
    using ProtoData = std::vector<uint8_t>;
    using ProtoCIter = ProtoData::const_iterator;
} // namespace protocol
