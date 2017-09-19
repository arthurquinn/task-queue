#include "dispatcher.h"


// FRIEND FUNCTION
void* listen_udp_socket(void* args) {
  Dispatcher* instance = static_cast<Dispatcher*>(args);
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

// PUBLIC MEMBERS

Dispatcher::Dispatcher() {
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

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
  const char* cmd = static_cast<const char*>(item->data());
  system(cmd);
  stage.push_back(item);
}

Dispatcher::~Dispatcher() {
  
}