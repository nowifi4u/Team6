#include "BringToTown.h"

double BringToTownTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& BringToTownTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Bring to town ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << "town=" << *(this->spTown_);// << ",";

    os << ']';
    return os;
}


