#ifndef TEAM6_UPGRADETRAIN_H
#define TEAM6_UPGRADETRAIN_H

#include "task.h"
#include "game/data/train.h"

class UpgradeTrainTask : public Task
{
private:
    std::shared_ptr<Trains::Train> spTrain_;

public:
    UpgradeTrainTask(GameController& gc, const std::shared_ptr<Trains::Train>& spTrain)
        :Task(gc, spTrain->idx), spTrain_(spTrain)
    {}

    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const;

    virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const;
};

#endif //TEAM6_UPGRADETRAIN_H
