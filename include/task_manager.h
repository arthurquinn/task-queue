#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "queue.h"
#include "dispatch.h"
#include "queue_item.h"

class TaskManager {
private:
  Dispatch dispatch;
  Queue queue;
public:
  TaskManager(const unsigned int max_tasks, const std::string& reconstruct_dir);
  ~TaskManager();
};

#endif