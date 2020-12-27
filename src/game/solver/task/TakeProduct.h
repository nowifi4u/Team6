#ifndef TEAM6_TAKEPRODUCT_H
#define TEAM6_TAKEPRODUCT_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "task.h"
#include "game/data/post.h"

class TakeProductTask : public Task
{
private:
    std::shared_ptr<Posts::Market> spMarket_;

public:
    TakeProductTask(GameController& gc, const std::shared_ptr<Posts::Market>& spMarket)
        :Task(gc, spMarket->point_idx), spMarket_(spMarket)
    { }

    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const;

    virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const;
};

#endif //TEAM6_TAKEPRODUCT_H
