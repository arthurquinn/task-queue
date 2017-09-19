#include "dispatcher.h"

void* thread_run(void* args) {
  thread_args_t* thread_args = static_cast<thread_args_t*>(args);
  QueueItem* qi = thread_args->queue_item;
  std::cout << static_cast<int>(qi->priority()) << std::endl;

  system("sleep 10 && ls -l");

  delete thread_args;
  pthread_exit(NULL);
}

// PRIVATE MEMBERS

void Dispatcher::start_in_thread(QueueItem* queue_item) {
  thread_args_t* args = new thread_args_t();
  args->instance = this;
  args->queue_item = queue_item;

  pthread_t thread;
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
  pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
  pthread_create(&thread, &thread_attr, &thread_run, static_cast<void*>(args));
  pthread_attr_destroy(&thread_attr);
  std::cout << "dispatched!!" << std::endl;
}

// PUBLIC MEMBERS

Dispatcher::Dispatcher() {
  
}

const int Dispatcher::count() const {
  return stage.size();
}

void Dispatcher::dispatch(QueueItem* item) {
  if (stage.size() >= STAGE_SIZE) {
    throw std::length_error("dispatcher: maximum number of tasks dispatched reached");
  }
  stage.push_back(item);
  start_in_thread(item);
}

Dispatcher::~Dispatcher() {
  
}