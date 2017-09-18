#include "dispatcher.h"

// PRIVATE MEMBERS

void Dispatcher::start_in_thread(TaskEntry te) {
  const char * cmd = static_cast<const char*>(te.task->data());
  pthread_t thread;
}

// PUBLIC MEMBERS

Dispatcher::Dispatcher() : stage(STAGE_SIZE) {
  
}

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
  TaskEntry te;
  te.task = item;
  te.status = NOT_STARTED;
  start_in_thread(te);
}

Dispatcher::~Dispatcher() {
  
}