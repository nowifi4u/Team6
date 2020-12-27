#include "task.h"
#include "game/solver/executor/executor.h"

void Task::reg(const std::shared_ptr<Executor>& e)
{
    SPDLOG_DEBUG("Registering new executor for Task {}", *this);
    executors_.push_back(std::shared_ptr<Executor>(e));
}

void Task::clear()
{
    SPDLOG_DEBUG("Removing all executors from", *this);
    executors_.clear();
}

std::basic_ostream<char>& Task::doprint(std::basic_ostream<char> &os) const
{
    os << "[Task "
       << "idx=" << idx_ << ", ";

    os << '\n';
    os << "executors=";
    {
        os << "[\n";
        for (const auto& e : executors_)
        {
            os << '\t' << *e << '\n';
        }
        os << "]";
    }

    os << "]";
    return os;
}

