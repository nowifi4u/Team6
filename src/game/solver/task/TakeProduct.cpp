#include "TakeProduct.h"

double TakeProductTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& TakeProductTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Take Product ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << "market=" << *(this->spMarket_);// << ",";

    os << ']';
    return os;
}

