// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <iostream>

using namespace std;

struct Range {
  uint64_t start;
  uint64_t end;
  int id;
};

vector<Range> fileCompact(vector<Range> blocks) {
  // loop down block by inverse order
  for (int j = blocks.size() - 1; j >= 0; j--) {
//   LOG(INFO) << "Evaluating swap for block: " << j << " " << blocks[j].start << " " << blocks[j].end << " " << blocks[j].id;
    if (blocks[j].id == -1) {
      continue;
    }
    for (int i = 0; i < j; i++) {
      if (blocks[i].id != -1) {
        continue;
      }
      if (blocks[j].end - blocks[j].start <= blocks[i].end - blocks[i].start) {
        if (blocks[j].end - blocks[j].start != blocks[i].end - blocks[i].start) {
          auto saveEnd = blocks[i].end;
          blocks[i].end = blocks[i].start + blocks[j].end - blocks[j].start;
          blocks[i].id = blocks[j].id;
          blocks[j].id = -1;
          Range newBlock{blocks[i].end + 1, saveEnd, -1};
          blocks.insert(blocks.begin() + i + 1, newBlock);
          j++;
        } else {
          blocks[i].id = blocks[j].id;
          blocks[j].id = -1;
        }
        break;
      }
    }
  }
  return blocks;
}

uint64_t checkSum(vector<Range> blocks) {
  uint64_t sum = 0;
  uint64_t start = 0;
  for (int i = 0; i < blocks.size(); i++) {
    if (blocks[i].id == -1) {
      continue;
    }
    uint64_t start = blocks[i].start;
    while (start <= blocks[i].end) {
      sum += start * blocks[i].id;
      start++;
    }
  }
  return sum;
}
int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    auto line = lines[0];
    LOG(INFO) << "Line: " << line;
    uint64_t num = 0;
    int fileId = 0;
    // make a struct with start and end and id
    vector<Range> blocks;
    uint64_t size = 0;
    for (auto& c : line) {
      int amount = c - '0';
      if (num % 2 == 0) {
        if (amount) {
          blocks.push_back({size, size + amount - 1, fileId});
        }
        fileId++;
      } else {
        if (amount) {
           blocks.push_back({size, size + amount - 1, -1});
        }
      }
      size += amount;
      num++;
    }
    auto compact = fileCompact(blocks);
    LOG(INFO) << "Checksum: " << checkSum(compact);
 
    LOG(INFO) << "Done!";
    return 0;
}
