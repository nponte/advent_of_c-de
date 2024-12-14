// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <queue>
#include <set>
#include <algorithm>
#include <map>

using namespace std;

struct Edge {
  int side;
  pair<int, int> start;
  pair<int, int> end;
};

vector<pair<int, int>> getRegions(int row, int col, vector<string> lines) {
  set<pair<int, int>> visited;
  queue<pair<int, int>> region;
  region.push(make_pair(row, col));
  while (!region.empty()) {
    pair<int, int> current = region.front();
    region.pop();
    if (visited.find(current) != visited.end()) {
      continue;
    }
    visited.insert(current);
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (pair<int, int> direction : directions) {
      int newRow = current.first + direction.first;
      int newCol = current.second + direction.second;
      if ((newRow >= 0 && newRow < lines.size() && newCol >= 0 && newCol < lines[0].size()) && (lines[newRow][newCol] == lines[current.first][current.second])) {
        region.push(make_pair(newRow, newCol));
      }
    }
  }
  vector<pair<int, int>> regionVector(visited.begin(), visited.end());
  LOG(INFO) << "Region size: " << regionVector.size();
  return regionVector;
}

uint64_t fenceCost(vector<pair<int, int>> region) {
  uint64_t area = region.size();
  uint64_t perimeter = 0;

  vector<Edge> edges;
  for (pair<int, int> point : region) {
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    // top edge
    if (find(region.begin(), region.end(), make_pair(point.first - 1, point.second)) == region.end()) {
      edges.push_back(Edge{1, point, {point.first, point.second + 1}});
    }
    // bottom edge
    if (find(region.begin(), region.end(), make_pair(point.first + 1, point.second)) == region.end()) {
      edges.push_back(Edge{-1, {point.first+1, point.second}, {point.first+1, point.second+1}});
    }
    // left edge
    if (find(region.begin(), region.end(), make_pair(point.first, point.second - 1)) == region.end()) {
      edges.push_back(Edge{1, {point.first, point.second}, {point.first+1, point.second}});
    }
    // right edge
    if (find(region.begin(), region.end(), make_pair(point.first, point.second + 1)) == region.end()) {
      edges.push_back(Edge{-1, {point.first, point.second+1}, {point.first+1, point.second+1}});
    }
  }
  vector<Edge> horizontal, vertical;
  for (auto &e : edges) {
    if (e.start.first == e.end.first) {
      horizontal.push_back(e);
    } else {
      vertical.push_back(e);
    }
  }
  vector<Edge> mergedEdges;
  {
      map<int, vector<Edge>> byRow;
      for (auto &e : horizontal) {
          int row = e.start.first;
          byRow[row].push_back(e);
      }

      // For each row, sort by starting column and merge
      for (auto &kv : byRow) {
          auto &rowEdges = kv.second;
          sort(rowEdges.begin(), rowEdges.end(), [](const Edge &a, const Edge &b) {
              return a.start.second < b.start.second;
          });

          Edge current = rowEdges[0];
          for (size_t i = 1; i < rowEdges.size(); i++) {
              if ((rowEdges[i].start.second <= current.end.second) && (rowEdges[i].side == current.side)) {
                  // Overlapping or adjacent
                  current.end.second = max(current.end.second, rowEdges[i].end.second);
              } else {
                  // No overlap, push current and reset
                  mergedEdges.push_back(current);
                  current = rowEdges[i];
              }
          }
          mergedEdges.push_back(current);
      }
  }

  // Merge vertical edges by column
  {
      // Group by column (x-coordinate)
      map<int, vector<Edge>> byCol;
      for (auto &e : vertical) {
          int col = e.start.second;
          byCol[col].push_back(e);
      }

      // For each column, sort by starting row and merge
      for (auto &kv : byCol) {
          auto &colEdges = kv.second;
          sort(colEdges.begin(), colEdges.end(), [](const Edge &a, const Edge &b) {
              return a.start.first < b.start.first;
          });

          Edge current = colEdges[0];
          for (size_t i = 1; i < colEdges.size(); i++) {
              if ((colEdges[i].start.first <= current.end.first) && (colEdges[i].side == current.side)) {
                  // Overlapping or adjacent
                  current.end.first = max(current.end.first, colEdges[i].end.first);
              } else {
                  // No overlap, push current and reset
                  mergedEdges.push_back(current);
                  current = colEdges[i];
              }
          }
          mergedEdges.push_back(current);
      }
  }

  for (auto &e : mergedEdges) {
    LOG(INFO) << "Merged edge: " << e.start.first << " " << e.start.second << " " << e.end.first << " " << e.end.second;
  }
  LOG(INFO) << "Area: " << area << " Perimeter: " << mergedEdges.size();
  return area * mergedEdges.size();
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();
    set<pair<int, int>> visited;
    vector<vector<pair<int, int>>> regions;
    for (int i = 0; i < lines.size(); i++) {
      for (int j = 0; j < lines[i].size(); j++) {
        if (visited.find(make_pair(i, j)) != visited.end()) {
          continue;
        }
        auto region = getRegions(i, j, lines);
        visited.insert(region.begin(), region.end());
        regions.push_back(region);
      }
    }

    uint64_t totalPrice = 0;
    for (auto region : regions) {
      totalPrice += fenceCost(region);
    }
    LOG(INFO) << "Total price: " << totalPrice;
    LOG(INFO) << "Done!";
    return 0;
}
