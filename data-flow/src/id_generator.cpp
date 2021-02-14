#include "data-flow/id_generator.hpp"
#include <atomic>

namespace dt::df
{
    class IdGenerator::Impl final
    {
    public:
        Impl()
            : id_{0}
        {
        }
        void reset(int value)
        {
            id_ = value;
        }
        int operator()()
        {
            return id_++;
        }

    private:
        std::atomic_int id_;
    };
    IdGenerator::IdGenerator()
        : impl_{new Impl{}}
    {
    }

    void IdGenerator::reset(int value)
    {
        impl_->reset(value);
    }

    int IdGenerator::operator()()
    {
        return (*impl_)();
    }
    IdGenerator::~IdGenerator()
    {
        delete impl_;
    }
} // namespace dt::df
