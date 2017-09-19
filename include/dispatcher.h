#ifndef DISPATCHER_H
#define DISPATCHER_H

#define STAGE_SIZE 4

#include "queue_item.h"

#include <pthread.h>
#include <vector>
#include <iostream>

void* thread_run(void* args);

class Dispatcher {
private:
  std::vector<QueueItem*> stage;
  void start_in_thread(QueueItem* queue_item);
public:
  Dispatcher();
  const int count() const;
  void dispatch(QueueItem* queue_item);
  ~Dispatcher();
};

typedef struct thread_args {
  Dispatcher* instance;
  QueueItem* queue_item;
} thread_args_t;

#endif