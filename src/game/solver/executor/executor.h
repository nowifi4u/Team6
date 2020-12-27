#ifndef TEAM6_EXECUTOR_H
#define TEAM6_EXECUTOR_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Task;
#include "game/solver/task/task.h"

class Executor {
protected:
    std::shared_ptr<Task> task_;

public:
    Executor() {}

    /***
     * Specify new task for executor.
     * @param task  New task.
     */
    void update(const std::shared_ptr<Task> &task);

    /// Execute specified task
    virtual void execute() const{};

    virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const;

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const Executor &e) { return e.doprint(os); }

};

#endif //TEAM6_EXECUTOR_H
