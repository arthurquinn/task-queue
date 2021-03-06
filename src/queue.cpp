#include "queue.h"

bool QueueItemComparator::operator()(const QueueItem* a, const QueueItem* b) {
  return a->priority() == b->priority() ? 
    a->push_index() > b->push_index() : 
    a->priority() < b->priority();
}

// PRIVATE FUNCTIONS

const std::vector<std::string> Queue::scan_qitems() const {
  int dir_err = 0;
  std::vector<std::string> qitem_vector;
  DIR * dir = opendir(_reconstruct_dir.c_str());
  if (!dir) {
    dir_err = create_dir(_reconstruct_dir);
    dir = opendir(_reconstruct_dir.c_str());
  }
  if (dir_err != 0 || !dir) {
    std::stringstream emsg;
    emsg << "unable to open directory '" << _reconstruct_dir << "'";
    throw std::ios_base::failure(emsg.str());
  } else {
    dirent * entry;
    while ((entry = readdir(dir)) != NULL) {
      if (has_suffix(entry->d_name, ".qi")) {
        qitem_vector.push_back(concat_filepath(_reconstruct_dir, entry->d_name));
      }
    }
    closedir(dir);
  }
  return qitem_vector;
}

// PUBLIC FUNCTIONS

Queue::Queue(const unsigned int max_len, const std::string& reconstruct_dir) :
  _reconstruct_dir(reconstruct_dir),
  _max_len(max_len)
{
  _push_cursor = 0;
  unsigned long _max_pc = 0;
  std::vector<std::string> qitem_files = scan_qitems();

  // Foreach queue_item file, try loading the saved queue_item into the queue
  std::vector<std::string>::iterator it;
  for (it = qitem_files.begin(); it < qitem_files.end(); it++) {
    QueueItem * qi = new QueueItem();
    if (qi->load(*it) == 0) {
      _push_cursor++;
      _pqueue.push(qi);
      _max_pc = std::max(_max_pc, qi->push_index() + 1);
    }
  }
  _push_cursor = _max_pc;
}

Queue::~Queue() {
  // delete _pqueue;
}

const int Queue::enqueue(QueueItem* queue_item) {
  std::stringstream strm;
  if (_pqueue.size() >= _max_len) {
    strm << "queue has reached maximum size: " << _max_len;
    throw std::length_error(strm.str());
  }
  if (queue_item->_is_enqueued) {
    strm << "item is already enqueued";
    throw std::logic_error(strm.str());
  }
  int retval = queue_item->write(_reconstruct_dir, _push_cursor++);
  _pqueue.push(queue_item);
  if (retval > 0) {
    retval = QUEUE_ERROR_WRITING_ITEM;
  }
  return retval;
}

const QueueItem* Queue::peek() const {
  if (_pqueue.empty()) {
    throw std::length_error(std::string("cannot peek an empty queue"));
  } else {
    return _pqueue.top();
  }
}

const int Queue::size() const {
  return _pqueue.size();
}

const unsigned long Queue::current_push_cursor() const {
  return _push_cursor;
}

QueueItem* Queue::dequeue() {
  QueueItem* item = NULL;
  if (_pqueue.empty()) {
    throw std::length_error(std::string("cannot dequeue an empty queue"));
  } else {
    item = _pqueue.top();
    _pqueue.pop();
  }
  return item;
}