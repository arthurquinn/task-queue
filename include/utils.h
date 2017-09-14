#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>

const int create_dir(const std::string& dir);

bool has_suffix(const std::string& s, const std::string& suffix);

std::string concat_filepath(const std::string& a, const std::string& b);

// generates a random filename located in directory dir
std::string gen_rand_filename(const std::string& dir);

#endif