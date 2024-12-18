// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <regex>
#include <set>
#include <iostream>
#include <queue>
#include <map>
#include <climits>

using namespace std;

struct Location {
  pair<int, int> loc;
  int cost;
};

void printGrid(vector<string>& grid) {
  for (int i = 0; i < grid.size(); i++) {
    cout << grid[i] << endl;
  }
}

int minStep(vector<string>& lines, int gridSize) {
  pair<int, int> end = {gridSize - 1, gridSize - 1};
  queue<Location> q;
  q.push({{0, 0}, 0});

  int lowestCost = INT_MAX;
  map<pair<int, int>, int> minCost;
  while (!q.empty()) {
    auto curr = q.front();
    q.pop();
    if (curr.loc == end) {
      if (curr.cost < lowestCost) {
        lowestCost = curr.cost;
      }
    }
    if (curr.loc.first < 0 || curr.loc.first >= gridSize || curr.loc.second < 0 || curr.loc.second >= gridSize || lines[curr.loc.first][curr.loc.second] == '#') {
      continue;
    }
    
    if (minCost.find(curr.loc) != minCost.end() && minCost[curr.loc] <= curr.cost) {
      continue;
    }
    minCost[curr.loc] = curr.cost;

    vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (auto dir : directions) {
      q.push({{curr.loc.first + dir.first, curr.loc.second + dir.second}, curr.cost + 1});
    }
  }
  return lowestCost;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    int gridSize = atoi(argv[2]);

    vector<string> grid;
    for (int i = 0; i < gridSize; i++) {
      string row = "";
      for (int j = 0; j < gridSize; j++) {
        row += ".";
      }
      grid.push_back(row);
    }

    int count = 0;
    for (auto line : lines) {
      regex r("([0-9]+),([0-9]+)");
      smatch m;
      regex_search(lines[count], m, r);
      int x = stoi(m[1]);
      int y = stoi(m[2]);
      grid[y][x] = '#';
      LOG(INFO) << lines[count];
      auto minSteps = minStep(grid, gridSize);
      if (minSteps == INT_MAX) {
        LOG(INFO) << "No path found!";
        LOG(INFO) << "line: " << lines[count];
        return 0;
      }
      count++;
    }

    LOG(INFO) << "minStep: " << minStep(grid, gridSize);
    for (int i = 0; i < gridSize; i++) {
      cout << grid[i] << endl;
    }
    LOG(INFO) << "Done!";
    return 0;
}
