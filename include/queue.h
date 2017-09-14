#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <dirent.h>

#include "queue_item.h"
#include "utils.h"

#define QUEUE_ERROR_LOADING_ITEM 1
#define QUEUE_ERROR_WRITING_ITEM 2
#define QUEUE_ERROR_REMOVING_ITEM 3

class Queue {

private:

  typedef std::priority_queue<QueueItem, std::vector<QueueItem>, QueueItem::QueueItemComparator> pqueue_type;

  unsigned int _max_len;
  pqueue_type * _pqueue;
  std::string _reconstruct_dir;

  // private helper to scan a directory for saved queue_item files
  std::vector<std::string> scan_qitems();

public:

  // Instantiate a Queue with a maximum length that will store and retrieve reconstruction files from reconstruct_dir
  // max_len: the maximum length of the queue
  // reconstruct_dir: the directory that all .qi queue_item files will be saved to and read from
  Queue(unsigned int max_len, const std::string& reconstruct_dir);

  // Enqueue a QueueItem into the queue and saves it to a queue_item .qi file
  // throws: std::length_error if the queue is full
  const int enqueue(QueueItem& queue_item);

  // Returns a reference to the QueueItem at the top of the queue without removing it from the queue
  // throws: std::length_error if the queue is empty
  const QueueItem& peek() const;

  // Returns the current size of the queue
  const int size() const;

  // Dequeue a QueueItem from the queue and delete its corresponding queue_item .qi file
  // throws: std::length_error if the queue is empty
  const int dequeue();

};

#endif