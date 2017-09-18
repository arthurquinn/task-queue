#include "dispatcher.h"

Dispatcher::Dispatcher() : stage(STAGE_SIZE) {
  
}

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
}

Dispatcher::~Dispatcher() {
  
}