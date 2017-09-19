#include "dispatcher.h"


// PUBLIC MEMBERS

Dispatcher::Dispatcher() {
  
}

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
  stage.push_back(item);

}

Dispatcher::~Dispatcher() {
  
}