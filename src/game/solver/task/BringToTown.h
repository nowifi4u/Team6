#ifndef TEAM6_BRINGTOTOWN_H
#define TEAM6_BRINGTOTOWN_H

#include "task.h"
#include "game/data/post.h"

class BringToTownTask : public Task
{
private:
    std::shared_ptr<Posts::Town> spTown_;

public:
    BringToTownTask(GameController& gc, const std::shared_ptr<Posts::Town>& spTown)
        :Task(gc, spTown->point_idx), spTown_(spTown)
    {}

    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const;

    virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const;
};
#endif //TEAM6_BRINGTOTOWN_H
