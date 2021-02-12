#pragma once
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT IdGenerator final
    {
    public:
        IdGenerator();
        int operator()();
        ~IdGenerator();

        IdGenerator(const IdGenerator &) = delete;
        IdGenerator &operator=(const IdGenerator &) = delete;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
