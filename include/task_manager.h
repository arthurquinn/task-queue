#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <cstring>

#include "queue.h"
#include "dispatch.h"
#include "queue_item.h"

class TaskManager {
private:
  Dispatch dispatch;
  Queue queue;
public:
  
  // Creates a new task manager
  // max_tasks: the maximum number of tasks allowed
  // reconstruct_dir: the directory that recovery files are stored
  TaskManager(const unsigned int max_tasks, const std::string& reconstruct_dir);

  // Move a queue_item onto the dispatch stage to begin running
  const int stage_task();

  // Enqueues a new task into the task_manager
  // priority: the priority of the task
  // command: the command to be executed
  const int push_task(const unsigned char priority, const char * command);

  // Deconstructs TaskManager
  /// TODO: this will need to clean up all queue_item objects
  ~TaskManager();
};

#endif