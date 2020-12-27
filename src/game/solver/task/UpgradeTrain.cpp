#include "UpgradeTrain.h"

double UpgradeTrainTask::getValue(const std::shared_ptr<Executor> &e, int dTime) const
{
    return std::numeric_limits<double>::min();
}

std::basic_ostream<char>& UpgradeTrainTask::doprint(std::basic_ostream<char> &os) const
{
    os << "[Upgrade Train ";

    os << "{";
    Task::doprint(os);
    os << "} ";

    os << "train_=" << *(this->spTrain_);// << ",";

    os << ']';
    return os;
}
