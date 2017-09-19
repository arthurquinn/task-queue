#include "dispatcher.h"

// PRIVATE MEMBERS

void Dispatcher::remove(const unsigned int tid) {
  std::vector<TaskEntry>::iterator it;
  for (it = stage.begin(); it < stage.end(); it++) {
    TaskEntry te = *it;
    if (te.task_id == tid) {
      stage.erase(it, it + 1);
      QueueItem* item = te.queue_item;
      std::cout << "removed: " << item->filepath() << std::endl;
      item->remove();
      delete item;
    }
  }
}

// PUBLIC MEMBERS

Dispatcher::Dispatcher() {
  if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw std::runtime_error("task manager: could not initiate udp socket");
  }

  struct sockaddr_in sock_addr;
  socklen_t len = sizeof(sock_addr);
  memset(reinterpret_cast<char*>(&sock_addr), 0, len);
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port = htons(0);


  if (bind(udp_socket, reinterpret_cast<struct sockaddr*>(&sock_addr), len) < 0) {
    throw std::runtime_error("task manager: could not bind udp_socket");
  }

  if (getsockname(udp_socket, reinterpret_cast<struct sockaddr*>(&sock_addr), &len) < 0) {
    throw std::runtime_error("task manager: unable to retrieve the established port number from socket info");
  }
  
  udp_port = ntohs(sock_addr.sin_port);
  current_task_id = 0;
}

void Dispatcher::check_cleanup() {
  struct pollfd poll_fd;
  poll_fd.fd = udp_socket;
  poll_fd.events = POLLIN;
  while (poll(&poll_fd, 1, 1) > 0) {
    std::cout << "poll found input" << std::endl;
    struct sockaddr_in remote_addr;
    socklen_t remote_len = sizeof(struct sockaddr_in);
    unsigned char buffer[UDP_BUFFER_SIZE];
    int recvlen;
    recvlen = recvfrom(udp_socket, buffer, UDP_BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr*>(&remote_addr), &remote_len);
    std::cout << "received " << recvlen << " bytes" << std::endl;
    if (recvlen > 0) {
      buffer[recvlen] = 0;
      std::cout << "received message: " << buffer << std::endl;
      const unsigned long tid = strtoui(reinterpret_cast<const char*>(buffer));
      remove(tid);
    }
  }
}

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
  const char* cmd = static_cast<const char*>(item->data());

  std::stringstream stream;
  stream << cmd << " " << current_task_id << " " << udp_port << " &";
  std::string complete_cmd = stream.str();
  system(complete_cmd.c_str());
  std::cout << "dispatched: " << complete_cmd << std::endl;

  TaskEntry te;
  te.queue_item = item;
  te.task_id = current_task_id++;
  stage.push_back(te);
}

Dispatcher::~Dispatcher() {
  
}