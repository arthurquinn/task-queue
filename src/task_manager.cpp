#include "task_manager.h"

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {

}


void TaskManager::run() {
  dispatcher.check_cleanup();
  if (dispatcher.count() < 4 && queue.size() > 0) {
    QueueItem* item = queue.dequeue();
    std::cout << "queue length: " << queue.size() << std::endl;
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