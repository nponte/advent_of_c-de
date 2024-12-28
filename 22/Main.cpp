// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <cmath>
#include <optional>
#include <map>
#include <set>

using namespace std;

map<pair<int, vector<int>>, uint64_t> bananaCache;

uint64_t mix(uint64_t secretNumber, uint64_t value) {
  return secretNumber ^ value;
}

uint64_t prune(uint64_t secretNumber) {
  return secretNumber % 16777216;
}

uint64_t evolve(uint64_t secretNumber) {
  uint64_t result = secretNumber * 64;
  secretNumber = mix(secretNumber, result);
  secretNumber = prune(secretNumber);
  result = floor(secretNumber / 32);
  secretNumber = mix(secretNumber, result);
  secretNumber = prune(secretNumber);
  result = secretNumber * 2048;
  secretNumber = mix(secretNumber, result);
  secretNumber = prune(secretNumber);
  return secretNumber;
}

vector<pair<int, optional<int>>> changes(uint64_t secretNumber) {
  vector<pair<int, optional<int>>> result;
  int lastDigit = secretNumber % 10;
  result.push_back({lastDigit, nullopt});
  for (int i = 0; i < 2000; i++) {
    secretNumber = evolve(secretNumber);
    int newDigit = secretNumber % 10;
    result.push_back({newDigit, newDigit - lastDigit});
    lastDigit = newDigit;
  }
  return result;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    vector<vector<pair<int, optional<int>>>> allChanges;
    for (auto line : lines) {
      auto secretNumber = stoull(line);
      auto priceChanges = changes(secretNumber);
      allChanges.push_back(priceChanges);
    }

    int vectorIndex = 0;
    set<vector<int>> allSequences;
    for (auto changeVector : allChanges) {
      for (int i = 1; i < changeVector.size() - 3; i++) {
        auto sequence = {
          changeVector[i].second.value(),
          changeVector[i+1].second.value(),
          changeVector[i+2].second.value(),
          changeVector[i+3].second.value()
        };
        allSequences.insert(sequence);
        if (bananaCache.find({vectorIndex, sequence}) == bananaCache.end()) {
          bananaCache[{vectorIndex, sequence}] = changeVector[i+3].first;
        }
      }
      vectorIndex++;
    }
    LOG(INFO) << "Done building cache! Found " << allSequences.size() << " sequences.";

    uint64_t maxBananas = 0;
    for (auto sequence : allSequences) {
      uint64_t bananas = 0;
      for (int j = 0; j < vectorIndex; j++) {
        if (bananaCache.find({j, sequence}) != bananaCache.end()) {
          bananas += bananaCache[{j, sequence}];
        }
      }
      maxBananas = max(maxBananas, bananas);
    }
    LOG(INFO) << "Bananas: " << maxBananas;
    LOG(INFO) << "Done!";
    return 0;
}
