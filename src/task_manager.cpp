#include "task_manager.h"

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {

}

const int TaskManager::stage_task() {
  if (queue.size() < 1) {
    throw std::length_error("task_manager: stage_task cannot stage from an empty queue");
  }
  if (dispatcher.count() >= 4) {
    throw std::length_error("task_manager: tried to stage a task to a full dispatcher");
  }
  QueueItem* item = queue.dequeue();
  dispatcher.dispatch(item);
  return 0;
}

const int TaskManager::push_task(const unsigned char priority, const char * command) {
  QueueItem* item = new QueueItem(priority, command, strlen(command) + 1);
  queue.enqueue(item);

  return 0;
}

TaskManager::~TaskManager() {
  while (queue.size() > 0) {
    const QueueItem* top = queue.dequeue();
    delete top; 
  }
}