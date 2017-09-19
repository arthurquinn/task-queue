#ifndef DISPATCHER_H
#define DISPATCHER_H

#define STAGE_SIZE 4
#define UDP_PORT_NO 12950
#define UDP_BUFFER_SIZE 2048

#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "queue_item.h"

typedef int socket_t;

// Listen in a loop for messages received on the udp socket
// This method listens in a thread and closes out processes that have written back their pids
void* listen_udp_socket(void*);

class Dispatcher {
private:
  std::vector<QueueItem*> stage;
  socket_t udp_socket;

  // Friend function that will run in a separate thread and listen on a udp socket for processes writing back their
  // process id's after they have finished execution
  friend void* listen_udp_socket(void*);
public:

  // Constructs a dispatcher that will handle process starting and ending 
  Dispatcher();

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