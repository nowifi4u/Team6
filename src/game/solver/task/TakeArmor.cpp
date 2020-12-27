#include "TakeArmor.h"

double TakeArmorTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& TakeArmorTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Take Armor ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << "storage=" << *(this->spStorage_);// << ",";

    os << ']';
    return os;
}


