
#ifndef TEAM6_DISPATCHER_H
#define TEAM6_DISPATCHER_H

#include <vector>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "game/solver/executor/executor.h"
#include "game/solver/task/task.h"
#include "game/solver/task/empty.h"
#include "game/solver/executor/TrainExecutor.h"

class TaskDispatcher{
private:
    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<std::shared_ptr<Executor>> executors;
    RouteManager rm;

public:
    void add(const std::shared_ptr<Task>& t)
    {
        SPDLOG_DEBUG("adding new task {}", *t);
        tasks.push_back(std::shared_ptr<Task>(t));
    }

    void add(const std::shared_ptr<Executor>& e)
    {
        SPDLOG_DEBUG("adding new Executor {}", *e);
        executors.push_back(std::shared_ptr<Executor>(e));
    }

    void dispatchAll()
    {
        executors.push_back(std::shared_ptr<Executor>((Executor*) (new TrainExecutor(rm))));
        SPDLOG_DEBUG("dispatching all tasks");
        for(const auto& t : tasks)
        {
            dispatch(t);
        }
    }

    void dispatch(const std::shared_ptr<Task>& t)
    {
        SPDLOG_DEBUG("dispatching task {}, use count: {}", *t, t.use_count());
        if(executors.size())
        {
            auto it = std::max_element(
                    executors.begin(), executors.end(),
                   [&t](const std::shared_ptr<Executor>& e1, const std::shared_ptr<Executor>& e2) ->  bool
                   {
                       return (t->getValue(e1,1) > t->getValue(e2,1));
                   });

            if(it != executors.end())
            {
                SPDLOG_TRACE("e: {}, t: {}", (*it).use_count(), t.use_count());
                (*it)->update(t);

            }
        }
        SPDLOG_DEBUG("t: {}", t.use_count());

        return;
    }


};

#endif //TEAM6_DISPATCHER_H
