// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"

using namespace std;

struct Key {
  vector<int> pattern;
};

struct Lock {
  vector<int> pattern;
};

Lock parseLock(vector<string> lines) {
  LOG(INFO) << "Parsing lock";
  for (auto line : lines) {
    LOG(INFO) << line;
  }
  Lock lock;
  vector<int> pattern;
  for (int col = 0; col < 5; col++) {
    int value = 0, row = 0;
    while ((row < 5) && (lines[row][col] == '#')) {
      value++;
      row++;
    }
    pattern.push_back(value);
  }
  lock.pattern = pattern;
  return lock;
}

Key parseKey(vector<string> lines) {
  LOG(INFO) << "Parsing key";
  for (auto line : lines) {
    LOG(INFO) << line;
  }
  Key key;
  vector<int> pattern;
  for (int col = 0; col < 5; col++) {
    int value = 0, row = 0;
    while ((row < 5) && (lines[4-row][col] == '#')) {
      value++;
      row++;
    }
    pattern.push_back(value);
  }
  key.pattern = pattern;
  return key;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    vector<Key> keys;
    vector<Lock> locks;
    int i = 0;
    while (i < lines.size()) {
      auto top = lines[i];
      auto bottom = lines[i+6];
      if (top == "#####") {
        auto pattern = vector<string>(lines.begin() + i + 1, lines.begin() + i + 6);
        locks.push_back(parseLock(pattern));
      } else if (bottom == "#####") {
        auto pattern = vector<string>(lines.begin() + i + 1, lines.begin() + i + 6);
        keys.push_back(parseKey(pattern));
      }
      i += 8;
    }
    LOG(INFO) << "Keys: " << keys.size() << " Locks: " << locks.size();

    int count = 0;
    for (auto key : keys) {
      for (auto lock : locks) {
        bool overlap = true;
        for (int i = 0; i < 5; i++) {
          if (key.pattern[i] + lock.pattern[i] > 5) {
            overlap = false;
            break;
          }
        }
        if (overlap) {
          count++;
        }
      }
    }
    LOG(INFO) << "Count: " << count;

    LOG(INFO) << "Done!";
    return 0;
}
