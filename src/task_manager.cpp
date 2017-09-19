#include "task_manager.h"

void* listen_udp_socket(void* args) {
  TaskManager* instance = static_cast<TaskManager*>(args);
  socket_t udp_socket = instance->udp_socket;

  struct sockaddr_in remote_addr;
  socklen_t remote_len = sizeof(struct sockaddr_in);
  unsigned char buffer[UDP_BUFFER_SIZE];
  int recvlen;

  for (;;) {
    std::cout << "listening for messages on port " << UDP_PORT_NO << std::endl;
    recvlen = recvfrom(udp_socket, buffer, UDP_BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr*>(&remote_addr), &remote_len);
    std::cout << "received " << recvlen << " bytes" << std::endl;
    if (recvlen > 0) {
      buffer[recvlen] = 0;
      std::cout << "received message: " << buffer << std::endl;
    }
  }

  std::cout << "leaving listen_udp_socket" << std::endl;
}

TaskManager::TaskManager(const unsigned int max_tasks, const std::string& recover_dir) : queue(max_tasks, recover_dir) {
  if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw std::runtime_error("task manager: could not initiate udp socket");
  }

  struct sockaddr_in sock_addr;
  memset(reinterpret_cast<char*>(&sock_addr), 0, sizeof(struct sockaddr_in));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port = htons(UDP_PORT_NO);

  if (bind(udp_socket, reinterpret_cast<struct sockaddr*>(&sock_addr), sizeof(struct sockaddr_in)) < 0) {
    throw std::runtime_error("task manager: could not bind udp_socket");
  }

  pthread_t thread;
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
  pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
  pthread_create(&thread, &thread_attr, &listen_udp_socket, static_cast<void*>(this));
  pthread_attr_destroy(&thread_attr);
}

const int TaskManager::stage_task() {
  if (queue.size() < 1) {
    throw std::length_error("task_manager: stage_task cannot stage from an empty queue");
  }
  if (dispatcher.count() >= 4) {
    throw std::length_error("task_manager: tried to stage a task to a full dispatcher");
  }
  QueueItem* item = queue.dequeue();
  dispatcher.dispatch(item);
  return 0;
}

const int TaskManager::push_task(const unsigned char priority, const char * command) {
  QueueItem* item = new QueueItem(priority, command, strlen(command) + 1);
  queue.enqueue(item);

  return 0;
}

TaskManager::~TaskManager() {
  while (queue.size() > 0) {
    const QueueItem* top = queue.dequeue();
    delete top; 
  }
}