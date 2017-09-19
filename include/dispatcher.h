#ifndef DISPATCHER_H
#define DISPATCHER_H

#define STAGE_SIZE 4

#include "queue_item.h"

#include <vector>
#include <iostream>

class Dispatcher {
private:
  std::vector<QueueItem*> stage;
public:
  Dispatcher();
  const int count() const;
  void dispatch(QueueItem* queue_item);
  ~Dispatcher();
};

#endif