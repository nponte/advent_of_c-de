// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <queue>
#include <map>
#include <iostream>
#include <climits>
#include <set>

using namespace std;

struct Node {
  pair<int, int> pos;
  int cost;
  int direction;
  vector<pair<int, int>> moves;
};

void printLines(vector<string>& lines, pair<int, int> pos, pair<int, int> end, int cost, int direction, vector<pair<int, int>> moves) {
  LOG(INFO) << "Cost: " << cost;
  for (int i = 0; i < lines.size(); i++) {
    for (int j = 0; j < lines[i].size(); j++) {
      if (pos.first == i && pos.second == j) {
        if (direction == 1) {
          cout << "^";
        } else if (direction == 2) {
          cout << ">";
        } else if (direction == 3) {
          cout << "v";
        } else {
          cout << "<";
        }
      } else if (end.first == i && end.second == j) {
        cout << "E";
      } else {
        for (auto move : moves) {
          if (move.first == i && move.second == j) {
            cout << "0";
            break;
          }
        }
        bool gootSeat = false;
        for (auto move : moves) {
          if (move.first == i && move.second == j) {
            gootSeat = true;
            break;
          }
        }
        if (gootSeat) {
          continue;
        }
        cout << lines[i][j];
      }
    }
    cout << endl;
  }
}

vector<pair<int, int>> minCost(vector<string>& lines, pair<int, int> pos, pair<int, int> end) {
  // direction Up=1 Left=2 Down=3 Right=4
  queue<Node> q;
  q.push({pos, 0, 2});

  vector<pair<int, int>> moves;
  int lowestCost = INT_MAX;
  map<pair<pair<int, int>, int>, int> minCost;
  while (!q.empty()) {
    auto curr = q.front();
    q.pop();
    if (curr.pos == end) {
      if (curr.cost < lowestCost) {
        lowestCost = curr.cost;
        moves.clear();
        for (auto move : curr.moves) {
          moves.push_back(move);
        }
      }
      if (curr.cost == lowestCost) {
        for (auto move : curr.moves) {
          moves.push_back(move);
        }
      }
    }
    if (curr.pos.first < 0 || curr.pos.first >= lines.size() || curr.pos.second < 0 || curr.pos.second >= lines[0].size() || lines[curr.pos.first][curr.pos.second] == '#') {
      continue;
    }
    pair<pair<int, int>, int> currKey = {curr.pos, curr.direction};
    if (minCost.find(currKey) != minCost.end() && minCost[currKey] < curr.cost) {
      continue;
    }
    auto newMoves = curr.moves;
    newMoves.push_back(curr.pos);
    // Rotate clockwise or counterclocwise, costs 1000 points
    auto pos = curr.pos;
    q.push({pos, curr.cost + 1000, (curr.direction + 1) % 4, newMoves});
    q.push({pos, curr.cost + 1000, (curr.direction + 3) % 4, newMoves});
    // move forwrd one time, costs 1 point
    pair<int, int> moveVec;
    if (curr.direction == 1) {
      moveVec = {-1, 0};
    } else if (curr.direction == 2) {
      moveVec = {0, 1};
    } else if (curr.direction == 3) {
      moveVec = {1, 0};
    } else {
      moveVec = {0, -1};
    }
    pos = {curr.pos.first + moveVec.first, curr.pos.second + moveVec.second};
    q.push({pos, curr.cost + 1, curr.direction, newMoves});

    minCost[currKey] = curr.cost;
    //printLines(lines, curr.pos, end, curr.cost, curr.direction);
  }
  return moves;
}


int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    pair<int, int> pos, end;
    for (int i = 0; i < lines.size(); i++) {
      for (int j = 0; j < lines[i].size(); j++) {
        if (lines[i][j] == 'S') {
          pos = {i, j};
        }
        if (lines[i][j] == 'E') {
          end = {i, j};
        }
      }
    }
    LOG(INFO) << "Start: " << pos.first << ", " << pos.second;
    LOG(INFO) << "End: " << end.first << ", " << end.second;

    auto moves = minCost(lines, pos, end);
    LOG(INFO) << "Moves: " << moves.size();

    set<pair<int, int>> visited;
    for (auto move : moves) {
      visited.insert(move);
    }
    LOG(INFO) << "Visited: " << visited.size() + 1;
    LOG(INFO) << "Done!";
    return 0;
}
