#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <cstring>

#include "queue.h"
#include "dispatcher.h"
#include "queue_item.h"

class TaskManager {
private:
  Dispatcher dispatcher;
  Queue queue;
public:
  
  // Creates a new task manager
  // max_tasks: the maximum number of tasks allowed
  // reconstruct_dir: the directory that recovery files are stored
  TaskManager(const unsigned int max_tasks, const std::string& reconstruct_dir);

  // Runs through a single iteration of the task manager
  // First checks if there are any available positions on the dispatcher stage
  // If there are, dequeue a queue item, start its task, and push it to the dispatcher stage
  // After, checks to see what pids have been returned to the udp port
  // If any pids have been returned, mark their staged task as complete and remove them from the stage
  void run();

  // Enqueues a new task into the task_manager
  // priority: the priority of the task
  // command: the command to be executed
  const int push_task(const unsigned char priority, const char * command);
  

  // Deconstructs TaskManager
  /// TODO: this will need to clean up all queue_item objects
  ~TaskManager();
};

#endif