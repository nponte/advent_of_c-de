// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <queue>

using namespace std;

uint64_t concatHelper(uint64_t a, uint64_t b) {
  string combined = to_string(a) + to_string(b);
  return stoull(combined);
}

bool testCalibration(queue<uint64_t> nums, uint64_t count, uint64_t desiredCount) {
  if (nums.empty()) {
    if (count == desiredCount) {
      return true;
    }
    return false;
  }
  uint64_t front = nums.front();
  nums.pop();
  if (count == 0) {
    return testCalibration(nums, front, desiredCount) || testCalibration(nums, front, desiredCount);
  }
  return testCalibration(nums, count + front, desiredCount) || testCalibration(nums, count * front, desiredCount) || testCalibration(nums, concatHelper(count, front), desiredCount);
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    uint64_t sum = 0;
    for (auto line : lines) {
      istringstream iss(line);
      char colon;
      uint64_t num, count;
      iss >> count >> colon;
      queue<uint64_t> nums;
      while (iss >> num) {
        nums.push(num);
      }
      if (testCalibration(nums, 0, count)) {
        LOG(INFO) << "Valid count: " << count;
        sum += count;
      }
    }
    LOG(INFO) << "Sum: " << sum;

    LOG(INFO) << "Done!";
    return 0;
}
