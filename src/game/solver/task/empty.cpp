#include "empty.h"

double EmptyTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& EmptyTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Empty Task ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << ']';
    return os;
}
