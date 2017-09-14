#include "utils.h"

const int create_dir(const std::string& dir) {
  std::string cmd("mkdir -p " + dir);
  return system(cmd.c_str());
}

bool has_suffix(const std::string& s, const std::string& suffix) {
  std::size_t found = s.find_last_of(".");
  if (found < s.size()) {
    return s.substr(found).compare(suffix) == 0;
  }
  return false;
}

/// TODO: make this more generic (i.e., deal with inputs containing and not containing '/' characters)
std::string concat_filepath(const std::string& a, const std::string& b) {
  std::stringstream stream;
  stream << a << "/" << b;
  return stream.str();
}

std::string gen_rand_filename(const std::string& dir) {
  struct stat buffer;
  std::string filepath;
  char buff[17];
  buff[16] = '\0';
  do {
    for (int i = 0; i < 16; i++) {
      buff[i] = (rand() % 10) + 48;
    }
    filepath = dir + "/queue_item_" + buff + ".qi";
  } while (stat(filepath.c_str(), &buffer) == 0);
  return filepath;
}