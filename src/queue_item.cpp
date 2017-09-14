#include "queue_item.h"

// PUBLIC MEMBERS

QueueItem::QueueItem(unsigned char priority, void * raw_data, unsigned int raw_data_len) {
  _is_enqueued = false;
  _priority = priority;
  _raw_data = raw_data;
  _raw_data_len = raw_data_len;
}

QueueItem::QueueItem() {
  _is_enqueued = false;
  _priority = 0;
  _raw_data = NULL;
  _raw_data_len = 0;
}

const void * QueueItem::data() const {
  return _raw_data;
}

const unsigned int QueueItem::data_len() const {
  return _raw_data_len;
}

const std::string& QueueItem::filepath() const {
  return _filepath;
}

const int QueueItem::priority() const {
  return _priority;
}

// PRIVATE MEMBERS

int QueueItem::load(const std::string& save_file) {
  int retval = 0;
  char * memblock;
  std::streampos size;
  std::ifstream instream(save_file.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
  if (instream.is_open()) {
    try {

      // read into memblock
      size = instream.tellg();
      memblock = new char[size];
      instream.seekg(0, std::ios::beg);
      instream.read(memblock,size);

      // construct QueueItem
      unsigned long * id = reinterpret_cast<unsigned long *>(memblock);
      if (*id == QUEUE_ITEM_ID) {
        unsigned char * p = reinterpret_cast<unsigned char *>(memblock+sizeof(unsigned long));
        unsigned int * len = reinterpret_cast<unsigned int *>(memblock+sizeof(unsigned long)+sizeof(unsigned char));
        _raw_data = reinterpret_cast<void *>(memblock+sizeof(unsigned long)+sizeof(unsigned char)+sizeof(unsigned int));
        _priority = *p;
        _raw_data_len = *len;
        _filepath = save_file;
      } else {
        retval = 1;
      }

    } catch (const std::exception& e) {
      retval = 1;
    }
  } else {
    retval = 1;
  }
  return retval;
}

int QueueItem::write(const std::string& save_dir) {
  int retval = 0;
  std::string filepath = gen_rand_filename(save_dir);
  std::ofstream outstream(filepath.c_str(), std::ios::out | std::ios::binary | std::ios::app);
  if (outstream.is_open()) {
    try {
      unsigned long id = QUEUE_ITEM_ID;
      outstream.write(reinterpret_cast<const char *>(&id),sizeof(unsigned long));
      outstream.write(reinterpret_cast<const char *>(&_priority),sizeof(unsigned char));
      outstream.write(reinterpret_cast<const char *>(&_raw_data_len),sizeof(unsigned int));
      outstream.write(reinterpret_cast<const char *>(_raw_data),_raw_data_len);
    } catch (const std::exception& e) {
      retval = 1;
    }
    outstream.close();
    _filepath = filepath;
  } else {
    retval = 1;
  }
  return retval;
}

int QueueItem::remove() {
  int retval = std::remove(_filepath.c_str());
  if (retval == 0) {
    _filepath.clear();
  }
  return retval;
}

void QueueItem::set_enqueued() {
  _is_enqueued = true;
}

bool QueueItem::QueueItemComparator::operator()(const QueueItem& a, const QueueItem& b) {
  return a._priority < b._priority;
}