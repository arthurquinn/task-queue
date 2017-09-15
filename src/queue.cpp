#include "queue.h"

Queue::QueueItemComparator::QueueItemComparator(const Queue& queue) : _queue(queue) { }

bool Queue::QueueItemComparator::operator()(const QueueItem& a, const QueueItem& b) {
  return a.priority() == b.priority() ? 
    (a._push_index - _queue._push_cursor) > (b._push_index - _queue._push_cursor) : 
    a.priority() < b.priority();
}

// PRIVATE FUNCTIONS

std::vector<std::string> Queue::scan_qitems() {
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

Queue::Queue(unsigned int max_len, const std::string& reconstruct_dir)
  : _pqueue(new queue_t(QueueItemComparator(*this))) {
  _push_cursor = 0;
  _max_len = max_len;
  _reconstruct_dir = reconstruct_dir;
  std::vector<std::string> qitem_files = scan_qitems();

  // Foreach queue_item file, try loading the saved queue_item into the queue
  std::vector<std::string>::iterator it;
  for (it = qitem_files.begin(); it < qitem_files.end(); it++) {
    QueueItem qi;
    if (qi.load(*it) == 0) {
      _pqueue->push(qi);
    }
  }
}

const int Queue::enqueue(QueueItem& queue_item) {
  std::stringstream strm;
  if (_pqueue->size() >= _max_len) {
    strm << "queue has reached maximum size: " << _max_len;
    throw std::length_error(strm.str());
  }
  if (queue_item._is_enqueued) {
    strm << "item is already enqueued";
    throw std::logic_error(strm.str());
  }
  _pqueue->push(queue_item);
  int retval = queue_item.write(_reconstruct_dir, _push_cursor++);
  if (retval > 0) {
    retval = QUEUE_ERROR_WRITING_ITEM;
  }
  return retval;
}

const QueueItem& Queue::peek() const {
  if (_pqueue->empty()) {
    throw std::length_error(std::string("cannot peek an empty queue"));
  } else {
    return _pqueue->top();
  }
}

const int Queue::size() const {
  return _pqueue->size();
}

const int Queue::dequeue() {
  int retval = 0;
  if (_pqueue->empty()) {
    throw std::length_error(std::string("cannot dequeue an empty queue"));
  } else {
    QueueItem item = _pqueue->top();
    if (item.remove() != 0) {
      retval = QUEUE_ERROR_REMOVING_ITEM;
    }
    _pqueue->pop();
  }
  return retval;
}