// nponte

#include <set>
#include <vector>
#include <glog/logging.h>
#include "Data.h"

using namespace std;

vector<int> moveGuard(int row, int col, int direction, vector<string> map, int oRow, int oCol) {
  // return row, col, direction vector
  int newRow, newCol;
  switch (direction) {
    case 0:
      newRow = row - 1;
      newCol = col;
      break;
    case 1:
      newRow = row;
      newCol = col + 1;
      break;
    case 2:
      newRow = row + 1;
      newCol = col;
      break;
    case 3:
      newRow = row;
      newCol = col - 1;
      break;
    default:
      LOG(FATAL) << "Invalid direction: " << direction;
  }

  vector<int> result;

  if (newRow < 0 || newRow >= map.size() || newCol < 0 || newCol >= map[0].size()) {
    return result;
  }

  if ((map[newRow][newCol] == '#') || (newRow == oRow && newCol == oCol)) {
    direction = (direction + 1) % 4;
    result.push_back(row);
    result.push_back(col);
    result.push_back(direction);
    return result;
  }
  result.push_back(newRow);
  result.push_back(newCol);
  result.push_back(direction);
  return result;
}

bool causesLoop(int row, int col, vector<string> map, int oRow, int oCol, int direction) {
  // make a set for visited
  set<string> visited;
  string location = to_string(row) + ", " + to_string(col) + ", " + to_string(direction);
  visited.insert(location);
  while (true) {
    vector<int> next = moveGuard(row, col, direction, map, oRow, oCol);
    if (next.size() == 0) {
      return false;
    }
    row = next[0];
    col = next[1];
    direction = next[2];
    location = to_string(row) + ", " + to_string(col) + ", " + to_string(direction);
    if (visited.find(location) != visited.end()) {
      return true;
    }
    visited.insert(location);
  }
  LOG(INFO) << "This should never happen!";
  return false;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto map = data.readLines();

    int sRow, sCol, direction;
    // find the starting point i.e. one of ['>', '<', '^', 'v']
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '>' || map[i][j] == '<' || map[i][j] == '^' || map[i][j] == 'v') {
                LOG(INFO) << "Found starting point: " << map[i][j] << " at " << i << ", " << j;
                sRow = i;
                sCol = j;
                if (map[i][j] == '^') {
                    direction = 0;
                } else if (map[i][j] == '>') {
                    direction = 1;
                } else if (map[i][j] == 'v') {
                    direction = 2;
                } else {
                    direction = 3;
                }
                break;
            }
        }
    }
    int count = 0;
    for (int i = 0; i < map.size(); i++) {
      for (int j = 0; j < map[i].size(); j++) {
        if ((sRow == i && sCol == j) || map[i][j] == '#') {
          continue;
        }
        count += causesLoop(sRow, sCol, map, i, j, direction);
      }
    }
    LOG(INFO) << "Number of loops: " << count;

    // make a set for visited
    LOG(INFO) << "Done!";
    return 0;
  }
