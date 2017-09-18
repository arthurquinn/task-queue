#ifndef DISPATCH_H
#define DISPATCH_H

#include "queue_item.h"

class Dispatch {
private:
  QueueItem stage[4];
public:
  Dispatch();
  ~Dispatch();
};

#endif