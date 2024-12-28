// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>
#include <climits>
#include <queue>
#include <functional>

using namespace std;

map<tuple<char, char, int>, uint64_t> memo;

class KeyPad {
  public:
    int deadRow_ = 0;
    int deadCol_ = 0;
    map<char, pair<int, int>> keypad_;
};

class DirectionalKeypad : public KeyPad {
 public:
  DirectionalKeypad() {
    keypad_ = {
        { '^', {0, 1} },
        { 'A', {0, 2} },
        { '<', {1, 0} },
        { 'v', {1, 1} },
        { '>', {1, 2} }
    };
    deadRow_ = 0;
    deadCol_ = 0;
  }
  ~DirectionalKeypad() {};
};

class NumericKeypad : public KeyPad {
 public:
  NumericKeypad() {
    keypad_ = {
        { '7', {0, 0} },
        { '8', {0, 1} },
        { '9', {0, 2} },
        { '4', {1, 0} },
        { '5', {1, 1} },
        { '6', {1, 2} },
        { '1', {2, 0} },
        { '2', {2, 1} },
        { '3', {2, 2} },
        { '0', {3, 1} },
        { 'A', {3, 2} }
    };
    deadRow_ = 3;
    deadCol_ = 0;
  }
  ~NumericKeypad() {};
};

vector<vector<char>> bfsAllPathsOnePad(KeyPad pad, char c1, char c2) {
//  LOG(INFO) << "Finding all paths from " << c1 << " to " << c2;
  if (c1 == c2) return {{'A'}};

  int maxRow = 0, maxCol = 0;
  for (auto &kv : pad.keypad_) {
    maxRow = max(maxRow, kv.second.first);
    maxCol = max(maxCol, kv.second.second);
  }
  auto [startRow, startCol] = pad.keypad_[c1];
  auto [endRow, endCol] = pad.keypad_[c2];

//  LOG(INFO) << "Start: " << startRow << ", " << startCol << " End: " << endRow << ", " << endCol << " Max: " << maxRow << ", " << maxCol;
  vector<vector<int>> dist(maxRow + 1, vector<int>(maxCol + 1, INT_MAX));
  vector<vector<vector<pair<int, int>>>> preds(maxRow+1, vector<vector<pair<int, int>>>(maxCol+1));

  auto validCell = [&](int row, int col) {
    if (row < 0 || row > maxRow || col < 0 || col > maxCol) return false;
    if (row == pad.deadRow_ && col == pad.deadCol_) return false;
    return true;
  };
  dist[startRow][startCol] = 0;
  queue<pair<int, int>> q;
  q.push({startRow, startCol});

  const vector<pair<int,int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  while (!q.empty()) {
    auto [row, col] = q.front();
    q.pop();
    int cd = dist[row][col];
    for (auto [dRow, dCol] : directions) {
      int newRow = row + dRow;
      int newCol = col + dCol;
      if (validCell(newRow, newCol)) {
        int nd = cd + 1;
        if (nd < dist[newRow][newCol]) {
          dist[newRow][newCol] = nd;
          preds[newRow][newCol] = {{row, col}};
          q.push({newRow, newCol});
        } else if (nd == dist[newRow][newCol]) {
          preds[newRow][newCol].push_back({row, col});
        }
      }
    }
  }

  vector<vector<pair<int, int>>> allPaths;

  function <void(pair<int,int>, vector<pair<int,int>> &)> buildPaths
    = [&](pair<int,int> cell, vector<pair<int,int>> &curPath) {
      if (cell.first == startRow && cell.second == startCol) {
        vector<std::pair<int,int>> path(curPath.rbegin(), curPath.rend());
        allPaths.push_back(path);
        return;
      }
      for (auto pred : preds[cell.first][cell.second]) {
        curPath.push_back(pred);
        buildPaths(pred, curPath);
        curPath.pop_back();
      }
    };

  vector<pair<int,int>> tmp;
  tmp.push_back({endRow, endCol});
  buildPaths({endRow, endCol}, tmp);

  auto makeDirs = [&](const vector<pair<int,int>> &pp){
      vector<char> dirs;
      for (size_t i=1; i<pp.size(); i++){
          int r1=pp[i-1].first, c1=pp[i-1].second;
          int r2=pp[i].first,  c2=pp[i].second;
          if (r2==r1) {
              if (c2>c1) dirs.push_back('>');
              else       dirs.push_back('<');
          } else {
              if (r2>r1) dirs.push_back('v');
              else       dirs.push_back('^');
          }
      }
      dirs.push_back('A');
      return dirs;
  };

  vector<vector<char>> finalPaths;
  for (auto &posPath : allPaths) {
    auto pathCandidate = makeDirs(posPath);
//    LOG(INFO) << "Path: " << string(pathCandidate.begin(), pathCandidate.end());
    finalPaths.push_back(pathCandidate);
  }
  return finalPaths;
}

KeyPad getPad(int depth) {
  return (depth == 25) ? (KeyPad)NumericKeypad() : (KeyPad)DirectionalKeypad();
}

uint64_t getCost(char c1, char c2, int depth) {
  if (memo.find({c1, c2, depth}) != memo.end()) {
    return memo[{c1, c2, depth}];
  }

  const KeyPad pad = getPad(depth);
  auto expansions = bfsAllPathsOnePad(pad, c1, c2);
  if (depth == 0) {
    uint64_t best = ULLONG_MAX;
    for (auto &p : expansions) {
      LOG(INFO) << "best: " << best << " p.size(): " << p.size();
      best = min(best, (uint64_t)p.size());
    }
    memo[{c1, c2, depth}] = best;
    return best;
  }

  uint64_t best = ULLONG_MAX;
  for (auto &p : expansions) {
      string partial(p.begin(), p.end());
      char current = 'A';
      uint64_t total = 0;
      for (char c : partial) {
        total += getCost(current, c, depth-1);
        current = c;
      }
      best = min(best, total);
  }
  memo[{c1, c2, depth}] = best;
  return best;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    NumericKeypad nPad;
    DirectionalKeypad dPad;
    uint64_t total = 0;
    for (auto line : lines) {
      char currChar = 'A';
      uint64_t length = 0;
      for (char c : line) {
        length += getCost(currChar, c, 25);
        currChar = c;
      }
      LOG(INFO) << "Processing line: " << line;
      uint64_t numericCode = stoll(line.substr(0, line.size() - 1));
      LOG(INFO) << "Numeric code: " << numericCode << " Sequence length: " << length;
      total += numericCode * length;
    }

    LOG(INFO) << "Total complexity: " << total;
    LOG(INFO) << "Done!";
    return 0;
}
