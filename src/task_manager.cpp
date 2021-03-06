#include "task_manager.h"

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {

}

const bool TaskManager::has_tasks() {
  std::cout << dispatcher.count() << std::endl;
  std::cout << queue.size() << std::endl;
  return dispatcher.count() > 0 || queue.size() > 0;
}


void TaskManager::run() {
  dispatcher.check_cleanup();
  if (dispatcher.count() < 4 && queue.size() > 0) {
    QueueItem* item = queue.dequeue();
    dispatcher.dispatch(item);
  }
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