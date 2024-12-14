// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <queue>
#include <map>

using namespace std;

map<pair<uint64_t, int>, uint64_t> cache;

uint64_t blink(uint64_t stone, int iteration) {
  if (cache.find({stone, iteration}) != cache.end()) {
    return cache[{stone, iteration}];
  }
  if (iteration == 0) {
    return 1;
  }
  uint64_t count = 0;
  string s = to_string(stone);
  if (stone == 0) {
    count = blink(1, iteration - 1);
  } else if (s.size() % 2 != 0) {
    count = blink(stone * 2024, iteration - 1);
  } else {
    auto num1 = (uint64_t)stoll(s.substr(0, s.size() / 2));
    auto num2 = (uint64_t)stoll(s.substr(s.size() / 2, s.size()));
    count = blink(num1, iteration - 1) + blink(num2, iteration - 1);
  }
  cache[{stone, iteration}] = count;
  return count;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    stringstream ss(lines[0]);
    uint64_t stones = 0;
    string stone;
    while (getline(ss, stone, ' ')) {
      stones += blink(stoll(stone), atoi(argv[2]));
    }

    LOG(INFO) << "Total stones: " << stones;

    LOG(INFO) << "Done!";
    return 0;
}
