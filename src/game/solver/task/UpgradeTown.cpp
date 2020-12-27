#include "UpgradeTown.h"

double UpgradeTownTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& UpgradeTownTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Upgrade Town ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << "town=" << *(this->spTown_);// << ",";

    os << ']';
    return os;
}
