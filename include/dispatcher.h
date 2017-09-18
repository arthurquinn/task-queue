#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "queue_item.h"

class Dispatcher {
private:
  QueueItem * stage[4];
public:
  Dispatcher();
  ~Dispatcher();
  const int size() const;
};

#endif