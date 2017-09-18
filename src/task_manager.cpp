#include "task_manager.h"

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {

}



const int TaskManager::push_task(const unsigned char priority, const char * command) {
  QueueItem * item = new QueueItem(priority, command, strlen(command) + 1);
  queue.enqueue(*item);

  return 1;
}

TaskManager::~TaskManager() {
  
}