#include "task_manager.h"

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {

}

TaskManager::~TaskManager() {
  
}