// nponte

#include <fstream>
#include <glog/logging.h>
#include "Data.h"

using namespace std;

vector<std::string> Data::readLines() {
  ifstream file(filePath_);
  std::string line;
  vector<std::string> lines;
  while (getline(file, line)) {
    lines.push_back(line);
  }
  LOG(INFO) << "Read " << lines.size() << " lines from " << filePath_;
  return lines;
}

void Data::print() {
  vector<std::string> lines = readLines();
  uint32_t i = 0;
  for (const std::string& line : lines) {
    LOG(INFO) << i << ": " << line;
    i++;
  }
  LOG(INFO) << "Printed " << lines.size() << " lines";
}
