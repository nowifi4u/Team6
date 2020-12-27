#include "executor.h"

void Executor::update(const std::shared_ptr<Task> &task)
{
    if (task_ != nullptr) {
        SPDLOG_DEBUG("Updating executor: {} to {}", *task_, *task);
    } else {
        SPDLOG_DEBUG("New task {} specified.", *task);
    }
    task_ = task;
}

std::basic_ostream<char>& Executor::doprint(std::basic_ostream<char> &os) const
{
    os << "[Executor "
       << "task.idx=" << task_->getIdx();

    os << "]";
    return os;
}
