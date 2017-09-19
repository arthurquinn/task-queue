#ifndef COMMAND_H
#define COMMAND_H

#include <cstring>

#include "queue_item.h"

class Command {
private:
  const unsigned int id;
  const char * cmd;

  // turn the command data into a stream for writing
  const char * to_stream();

  // construct command from a stream read in from a file
  Command(const char * stream);

  // QueueItem needs access to call to_stream and constructor from stream
  friend class QueueItem;
public:
  Command(const unsigned int id, const char * cmd);
  ~Command();
};

#endif
