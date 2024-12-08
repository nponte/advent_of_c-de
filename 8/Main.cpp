// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>
#include <set>

using namespace std;

vector<pair<uint64_t, uint64_t>> placeOverlap(
    uint64_t rowA, uint64_t colA, uint64_t rowB, uint64_t colB, uint64_t height, uint64_t width
) {
  vector<pair<uint64_t, uint64_t>> overlaps;
  LOG(INFO) << "Placing overlap between (" << rowA << ", " << colA << ") and (" << rowB << ", " << colB << ")";

  uint64_t rowFreq = rowB - rowA;
  uint64_t colFreq = colB - colA;
  uint64_t newRowA = rowA - (rowB - rowA);
  uint64_t newColA = colA - (colB - colA);
  while (newRowA >= 0 && newRowA < height && newColA >= 0 && newColA < width) {
    LOG(INFO) << "Overlap at (" << newRowA << ", " << newColA << ")";
    overlaps.push_back(make_pair(newRowA, newColA));
    newRowA -= rowFreq;
    newColA -= colFreq;
  }

  uint64_t rowFreqB = rowA - rowB;
  uint64_t colFreqB = colA - colB;
  uint64_t newRowB = rowB - (rowA - rowB);
  uint64_t newColB = colB - (colA - colB);
  while (newRowB >= 0 && newRowB < height && newColB >= 0 && newColB < width) {
    LOG(INFO) << "Overlap at (" << newRowB << ", " << newColB << ")";
    overlaps.push_back(make_pair(newRowB, newColB));
    newRowB -= rowFreqB;
    newColB -= colFreqB;
  }
  return overlaps;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    set<pair<uint64_t, uint64_t>> overlaps;
    map<char, vector<pair<uint64_t, uint64_t>>> prev;
    for (int i = 0; i < lines.size(); i++) {
      for (int j = 0; j < lines[i].size(); j++) {
        if (lines[i][j] == '.') {
          continue;
        }
        overlaps.insert(make_pair(i, j));
        for (pair<uint64_t, uint64_t> old : prev[lines[i][j]]) {
          auto newOverlaps = placeOverlap(i, j, old.first, old.second, lines.size(), lines[i].size());
          overlaps.insert(newOverlaps.begin(), newOverlaps.end());
        }
        prev[lines[i][j]].push_back(make_pair(i, j));
      }
    }

    for (pair<uint64_t, uint64_t> overlap : overlaps) {
      lines[overlap.first][overlap.second] = 'X';
    }
    for (auto line : lines) {
      LOG(INFO) << line;
    }
    LOG(INFO) <<  "Overlaps: " << overlaps.size();
    LOG(INFO) << "Done!";
    return 0;
}
