#ifndef TEAM6_TASK_H
#define TEAM6_TASK_H

#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class GameController;
class Executor;

class Task
{
protected:
    std::vector<std::shared_ptr<Executor>> executors_;
    GameController& gc_;
    uint32_t idx_;

public:
    Task(GameController& gc, uint32_t idx)
            :gc_(gc), idx_(idx)
    {}

    /***
     * Only registers new one executor for this Task.
     * @param e Executor
     */
    void reg(const std::shared_ptr<Executor>& e);

    /***
     * Clears all executors of this task.
     */
    void clear();

    /***
     * Calculate value of Task, with given executor and execution time.
     * @param e Executor of this task.
     * @param dTime task execution time, in turns.
     * @return value of this Task, if it will be executed
     * by Executor and takes dTime time to complete.
     */
    virtual double getValue(const std::shared_ptr<Executor>& e, int dTime) const = 0;

    uint32_t getIdx() const { return idx_; }

    virtual std::basic_ostream<char> & doprint(std::basic_ostream<char> &os) const;

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const Task &t)
    {
        return t.doprint(os);
    }

};

#endif //TEAM6_TASK_H
