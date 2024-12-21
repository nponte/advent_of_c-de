// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>

using namespace std;

map<string, uint64_t> count;

map<string, bool> designCache;

uint64_t designPossible(string design, vector<string> towels) {
  if (designCache.find(design) != designCache.end()) {
    return designCache[design];
  }
  if (design.size() == 0) {
    return true;
  }
  bool possible = false;
  for (auto towel : towels) {
    if (towel.size() > design.size()) {
      continue;
    }
    auto d = design.substr(0, towel.size());
    if (d == towel) {
      possible |= designPossible(design.substr(towel.size(), design.size()), towels);
    }
  }
  designCache[design] = possible;
  return possible;
}

uint64_t numberOfDesigns(string design, vector<string> towels) {
  if (count.find(design) != count.end()) {
    return count[design];
  }
  if (design.size() == 0) {
    return 1;
  }
  uint64_t designs = 0;
  for (auto towel : towels) {
    if (towel.size() > design.size()) {
      continue;
    }
    auto d = design.substr(design.size() - towel.size(), towel.size());
    if (d == towel) {
      designs += numberOfDesigns(design.substr(0, design.size() - towel.size()), towels);
    } 
  }
  count[design] = designs;
  return designs;
}

vector<string> parseTowels(string line) {
  vector<string> towels;
  string towel;
  for (auto c : line) {
    if (c == ' ') {
      towel = "";
    } else if (c == ',') {
      towels.push_back(towel);
    } else {
      towel += c;
    }
  }
  towels.push_back(towel);
  for (auto towel : towels) {
    LOG(INFO) << "Towel: " << towel;
  }
  LOG(INFO) << "Towels: " << towels.size();
  return towels;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    uint64_t count = 0;
    auto towels = parseTowels(lines[0]);
    for (int i = 2; i < lines.size(); i++) {
      count += numberOfDesigns(lines[i], towels);
    }
    LOG(INFO) << "Count: " << count;
    LOG(INFO) << "Done!";
    return 0;
}
