#ifndef TEAM6_TAKEARMOR_H
#define TEAM6_TAKEARMOR_H

#include "task.h"
#include "game/data/post.h"

class TakeArmorTask : public Task
{
private:
    std::shared_ptr<Posts::Market> spStorage_;

public:
    TakeArmorTask(GameController& gc, const std::shared_ptr<Posts::Market>& spStorage)
        :Task(gc, spStorage->point_idx), spStorage_(spStorage)
    {}

    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const;

    virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const;

};

#endif //TEAM6_TAKEARMOR_H
