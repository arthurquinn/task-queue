#include "command.h"

// PRIVATE MEMBERS

const char * Command::to_stream() {
  size_t uintsize = sizeof(const unsigned int);
  size_t strsize = strlen(cmd);
  size_t size = uintsize + strsize + 1;
  char * buffer = new char[size];
  memcpy(buffer, &id, uintsize);
  memcpy(buffer + uintsize, cmd, strsize);
  buffer[size - 1] = 0;
  return buffer;
}

const unsigned int Command::extract_id(const char * stream) {
  return 0;
}

const char * Command::extract_cmd(const char * stream) {
  return "";
}

Command::Command(const char * stream) : id(extract_id(stream)), cmd(extract_cmd(stream)) { }

// PUBLIC MEMBERS

Command::Command(const unsigned int id, const char * cmd) : id(id), cmd(cmd) { }

Command::~Command() {

}