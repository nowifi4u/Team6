#ifndef TEAM6_EMPTY_H
#define TEAM6_EMPTY_H

#include <numeric>

#include "task.h"
#include <spdlog/fmt/ostr.h>

class EmptyTask : public Task{
public:
    EmptyTask(GameController& gc)
        :Task(gc,-1)
    {}

    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const;

    virtual std::basic_ostream<char> & doprint(std::basic_ostream<char> &os) const;
};

#endif //TEAM6_EMPTY_H
