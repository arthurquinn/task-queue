#ifndef DISPATCHER_H
#define DISPATCHER_H

#define STAGE_SIZE 4

#include "queue_item.h"

#include <pthread.h>
#include <vector>
#include <iostream>

enum Status { NOT_STARTED, RUNNING, FAILED, SUCCESS };

typedef struct task_entry {
  QueueItem* task;
  Status status;
} TaskEntry;

class Dispatcher {
private:
  std::vector<TaskEntry> stage;
public:
  Dispatcher();
  const int count() const;
  void dispatch(QueueItem* queue_item);
  ~Dispatcher();
};

#endif