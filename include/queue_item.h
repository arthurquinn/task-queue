#ifndef QUEUE_ITEM_H
#define QUEUE_ITEM_H

#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>

#include "utils.h"


/// TODO: Make the stdout messages exceptions that the queue catches and does not interrupt user code

#define QUEUE_ITEM_ID 0x2d881b241b09ffe1

class QueueItem {

private:

  // Allow Queue to access private data members of QueueItem
  friend class Queue;

  bool _is_enqueued;
  void * _raw_data;
  unsigned int _raw_data_len;
  unsigned char _priority;
  unsigned long _push_index;
  std::string _filepath;

public:

  // Constructor to create a new QueueItem
  // priority: value 0-255 for the priority of this item within the queue
  // raw_data: pointer to raw data
  // raw_data_len: length of the raw data in bytes
  QueueItem(unsigned char priority, const void * raw_data, unsigned int raw_data_len);

  // initializes an empty QueueItem
  QueueItem();

  // destructor
  ~QueueItem();

  // Reconstructs a QueueItem from a binary file
  // save_file: the location of the file to load the QueueItem from
  int load(const std::string& save_file);
  
  // Writes a QueueItem to a binary file that can later be used to reconstruct the QueueItem
  // filepath: the directory that the queue_item will be written to (should be the same directory as all other queue_items in the queue)
  int write(const std::string& save_dir, const unsigned long push_index);

  // get pointer to raw data
  const void * data() const;

  // get length of raw data
  const unsigned int data_len() const;

  // Returns the filepath used to save this queue_item
  const std::string& filepath() const;

  // Returns the specified priority for this queue_item
  const unsigned char priority() const;

  const unsigned long push_index() const;

  // Removes the save file associated with this queue_item
  int remove();

};


#endif