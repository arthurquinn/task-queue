#ifndef DISPATCHER_H
#define DISPATCHER_H

#define STAGE_SIZE 4
#define UDP_BUFFER_SIZE 2048

#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <string>

#include "queue_item.h"
#include "utils.h"

typedef int socket_t;

typedef struct task_entry {
  QueueItem* queue_item;
  unsigned int task_id;
} TaskEntry;

class Dispatcher {
private:
  unsigned int current_task_id;
  std::vector<TaskEntry> stage;
  socket_t udp_socket;

  // Remove a TaskEntry from the stage by its task_id and delete its corresponing QueueItem
  // This method frees room for more QueueItems to be dispatched from the Queue
  void remove(const unsigned int tid);
public:

  // Constructs a dispatcher that will handle process starting and ending 
  Dispatcher();

  // Checks if any processes have reported that they are finished
  // If there are finished processes, remove them from the stage and destroy the QueueItem
  void check_cleanup();

  // Returns the number of currently staged queue items
  // Queue items on the stage are items that are either starting to run, running, or have finished but have not yet been
  // unstages by the dispatcher
  const int count() const;

  // Dispatch a queue item to begin running its target process
  // This will call the command specified by the queue item
  void dispatch(QueueItem* queue_item);

  ~Dispatcher();
};

#endif