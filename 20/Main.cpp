// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>
#include <queue>
#include <climits>
#include <set>

using namespace std;

struct Map {
  vector<vector<char>> map;
  int width;
  int height;
  pair<int, int> start;
  pair<int, int> end;
};

vector<vector<int>> bfs(Map map) {
  vector<vector<int>> distances(map.height, vector<int>(map.width, INT_MAX));
  queue<pair<int, int>> q;
  auto [endR, endC] = map.end;
  distances[endR][endC] = 0;
  q.push(map.end);
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    if (r == map.start.first && c == map.start.second) {
      break;
    }
    int d = distances[r][c];
    vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (auto [dr, dc] : directions) {
      int newR = r + dr;
      int newC = c + dc;
      if (newR < 0 || newR >= map.height || newC < 0 || newC >= map.width) {
        continue;
      }
      if (map.map[newR][newC] == '#') {
        continue;
      }
      if (distances[newR][newC] == INT_MAX) {
        distances[newR][newC] = d + 1;
        q.push({newR, newC});
      }
    }
  }
  return distances;
}

uint64_t cheatScanning(Map map, vector<vector<int>> distances, int maxCheat) {
  uint64_t count = 0;
  for (int r = 0; r < map.height; r++) {
    for (int c = 0; c < map.width; c++) {
      if (map.map[r][c] != '.') {
        continue;
      }
      int cDist = distances[r][c];
      if (cDist == INT_MAX) {
        continue;
      }
      for (int dy = 0; dy <= maxCheat; dy++) {
        int dxStart = (dy == 0 ? 1 : (dy - maxCheat));
        int dxEnd = (maxCheat - dy);
        for (int dx = dxStart; dx <= dxEnd; dx++) {
          int nr = r + dy;
          int nc = c + dx;
          if (nr < 0 || nr >= map.height || nc < 0 || nc >= map.width) {
            continue;
          }
          if (map.map[nr][nc] != '.') {
            continue;
          }
          int nDist = distances[nr][nc];
          if (nDist == INT_MAX) {
            continue;
          }
          int manhat = abs(dx) + dy;
          int cheatVal = abs(nDist - cDist) - manhat;
          if (cheatVal >= 100) {
            count++;
          }
        }
      }
    }
  }
  return count;
}

Map readMap(vector<string> lines) {
  Map map;
  map.height = lines.size();
  map.width = lines[0].size();
  map.map = vector<vector<char>>(map.height, vector<char>(map.width));
  for (int i = 0; i < map.height; i++) {
    for (int j = 0; j < map.width; j++) {
      if (lines[i][j] == 'S') {
        map.map[i][j] = '.';
        map.start = make_pair(i, j);
      } else if (lines[i][j] == 'E') {
        map.map[i][j] = '.';
        map.end = make_pair(i, j);
      } else {
        map.map[i][j] = lines[i][j];
      }
    }
  }
  return map;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    auto map = readMap(lines);
    auto distances = bfs(map);
    uint64_t count = cheatScanning(map, distances, 20);
    LOG(INFO) << "Number of paths that save 100 or more time: " << count;
    LOG(INFO) << "Done!";
    return 0;
}
