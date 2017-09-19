#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "queue.h"
#include "dispatcher.h"
#include "queue_item.h"

#define UDP_PORT_NO 12950
#define UDP_BUFFER_SIZE 2048

typedef int socket_t;

// Listen in a loop for messages received on the udp socket
// This method listens in a thread and closes out processes that have written back their pids
void* listen_udp_socket(void*);

class TaskManager {
private:
  socket_t udp_socket;
  Dispatcher dispatcher;
  Queue queue;

  friend void* listen_udp_socket(void*);
public:
  
  // Creates a new task manager
  // max_tasks: the maximum number of tasks allowed
  // reconstruct_dir: the directory that recovery files are stored
  TaskManager(const unsigned int max_tasks, const std::string& reconstruct_dir);

  // Move a queue_item onto the dispatch stage to begin running
  const int stage_task();

  // Enqueues a new task into the task_manager
  // priority: the priority of the task
  // command: the command to be executed
  const int push_task(const unsigned char priority, const char * command);

  // Deconstructs TaskManager
  /// TODO: this will need to clean up all queue_item objects
  ~TaskManager();
};

#endif