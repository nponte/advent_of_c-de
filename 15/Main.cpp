// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <iostream>

using namespace std;
enum class Direction { UP, DOWN, LEFT, RIGHT };
enum class Tile { EMPTY, WALL, LBOX, RBOX, ROBOT };
struct Map {
  vector<vector<Tile>> map;
  pair<int, int> robot;
};

bool canPush(Map& map, pair<int, int> lBox, pair<int, int> rBox, pair<int, int> movVec) {
  auto newlBox = make_pair(lBox.first + movVec.first, lBox.second + movVec.second);
  auto newrBox = make_pair(rBox.first + movVec.first, rBox.second + movVec.second);

  // if horizontal
  if (movVec.first == 0) {
    if (movVec.second == -1) {
      if (map.map[newlBox.first][newlBox.second] == Tile::WALL) {
        return false;
      }
      if (map.map[newlBox.first][newlBox.second] == Tile::RBOX) {
        if (!canPush(map, {newlBox.first, newlBox.second - 1}, newlBox, movVec)) {
          return false;
        }
      }
    }
    if (movVec.second == 1) {
      if (map.map[newrBox.first][newrBox.second] == Tile::WALL) {
        return false;
      }
      if (map.map[newrBox.first][newrBox.second] == Tile::LBOX) {
        if (!canPush(map, newrBox, {newrBox.first, newrBox.second +1}, movVec)) {
          return false;
        }
      }
    }
  }

  // if vertical
  if (movVec.second == 0) {
    if ((map.map[newlBox.first][newlBox.second] == Tile::WALL) || (map.map[newrBox.first][newrBox.second] == Tile::WALL)) {
      return false;
    }
    if (map.map[newlBox.first][newlBox.second] == Tile::LBOX) {
      if (!canPush(map, newlBox, newrBox, movVec)) {
        return false;
      }
    }
    if (map.map[newlBox.first][newlBox.second] == Tile::RBOX) {
      if (!canPush(map, {newlBox.first, newlBox.second-1}, newlBox, movVec)) {
        return false;
      }
    }
    if (map.map[newrBox.first][newrBox.second] == Tile::LBOX) {
      if (!canPush(map, newrBox, {newrBox.first, newrBox.second+1}, movVec)) {
        return false;
      }
    }
  }
  return true;
};

void pushBox(Map& map, pair<int, int> lBox, pair<int, int> rBox, pair<int, int> movVec) {
  auto newlBox = make_pair(lBox.first + movVec.first, lBox.second + movVec.second);
  auto newrBox = make_pair(rBox.first + movVec.first, rBox.second + movVec.second);

  // if horizontal
  if (movVec.first == 0) {
    if (movVec.second == -1) {
      if (map.map[newlBox.first][newlBox.second] == Tile::RBOX) {
        pushBox(map, {newlBox.first, newlBox.second - 1}, newlBox, movVec);
      }
    }
    if (movVec.second == 1) {
      if (map.map[newrBox.first][newrBox.second] == Tile::LBOX) {
        pushBox(map, newrBox, {newrBox.first, newrBox.second +1}, movVec);
      }
    }
    map.map[newlBox.first][newlBox.second] = Tile::LBOX;
    map.map[newrBox.first][newrBox.second] = Tile::RBOX;
  }

  // if vertical
  if (movVec.second == 0) {
    if (map.map[newlBox.first][newlBox.second] == Tile::LBOX) {
      pushBox(map, newlBox, newrBox, movVec);
    }
    if (map.map[newlBox.first][newlBox.second] == Tile::RBOX) {
      pushBox(map, {newlBox.first, newlBox.second-1}, newlBox, movVec);
      map.map[newlBox.first][newlBox.second-1] = Tile::EMPTY;
    }
    if (map.map[newrBox.first][newrBox.second] == Tile::LBOX) {
      pushBox(map, newrBox, {newrBox.first, newrBox.second+1}, movVec);
      map.map[newrBox.first][newrBox.second+1] = Tile::EMPTY;
    }
    map.map[newlBox.first][newlBox.second] = Tile::LBOX;
    map.map[newrBox.first][newrBox.second] = Tile::RBOX;
  }
};

Map parseMap(vector<string> lines) {
  Map map;
  for (int i = 0; i < lines.size(); i++) {
    vector<Tile> row;
    for (int j = 0; j < lines[i].size(); j++) {
      auto c = lines[i][j];
      if (c == '.') {
        row.push_back(Tile::EMPTY);
        row.push_back(Tile::EMPTY);
      } else if (c == '#') {
        row.push_back(Tile::WALL);
        row.push_back(Tile::WALL);
      } else if (c == 'O') {
        row.push_back(Tile::LBOX);
        row.push_back(Tile::RBOX);
      } else if (c == '@') {
        map.robot = {i, 2*j};
        row.push_back(Tile::ROBOT);
        row.push_back(Tile::EMPTY);
      }
    }
    map.map.push_back(row);
  }
  return map;
};

vector<Direction> parseMovements(vector<string> lines) {
  vector<Direction> movements;
  for (auto line : lines) {
    for (auto c : line) {
      if (c == '^') {
        movements.push_back(Direction::UP);
      } else if (c == 'v') {
        movements.push_back(Direction::DOWN);
      } else if (c == '<') {
        movements.push_back(Direction::LEFT);
      } else if (c == '>') {
        movements.push_back(Direction::RIGHT);
      }
    }
  }
  return movements;
};

void printMap(Map map) {
  for (auto row : map.map) {
    for (auto tile : row) {
      if (tile == Tile::EMPTY) {
        cout << ".";
      } else if (tile == Tile::WALL) {
        cout << "#";
      } else if (tile == Tile::ROBOT) {
        cout << "@";
      } else if (tile == Tile::LBOX) {
        cout << "[";
      } else if (tile == Tile::RBOX) {
        cout << "]";
      }
    }
    cout << endl;
  }
};

void applyMovement(Map& map, Direction d) {
  pair<int, int> movVec;
  if (d == Direction::UP) {
    movVec = {-1, 0};
  } else if (d == Direction::DOWN) {
    movVec = {1, 0};
  } else if (d == Direction::LEFT) {
    movVec = {0, -1};
  } else if (d == Direction::RIGHT) {
    movVec = {0, 1};
  }

  auto newRobot = make_pair(map.robot.first + movVec.first, map.robot.second + movVec.second);
  if (map.map[newRobot.first][newRobot.second] == Tile::WALL) {
    return;
  } else if (map.map[newRobot.first][newRobot.second] == Tile::EMPTY) {
    map.map[map.robot.first][map.robot.second] = Tile::EMPTY;
    map.map[newRobot.first][newRobot.second] = Tile::ROBOT;
    map.robot = newRobot;
    return;
  }
  pair<int, int> rBox, lBox;
  if (map.map[newRobot.first][newRobot.second] == Tile::RBOX) {
    rBox = newRobot;
    lBox = {newRobot.first, newRobot.second - 1};
  }
  if (map.map[newRobot.first][newRobot.second] == Tile::LBOX) {
    lBox = newRobot;
    rBox = {newRobot.first, newRobot.second + 1};
  }

  if (!canPush(map, lBox, rBox, movVec)) {
    return;
  }
  pushBox(map, lBox, rBox, movVec);
  map.map[map.robot.first][map.robot.second] = Tile::EMPTY;
  map.robot = newRobot;
  map.map[map.robot.first][map.robot.second] = Tile::ROBOT;
  if (movVec.second == 0) {
    if (lBox == newRobot) {
      map.map[rBox.first][rBox.second] = Tile::EMPTY;
    }
    if (rBox == newRobot) {
      map.map[lBox.first][lBox.second] = Tile::EMPTY;
    }
  }
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    int empty_line = 0;
    for (int i = 0; i < lines.size(); i++) {
      auto line = lines[i];
      if (line.size() == 0) {
          empty_line = i;
          break;
      }
    }
    vector<string> before(lines.begin(), lines.begin() + empty_line);
    vector<string> after(lines.begin() + empty_line + 1, lines.end());

    auto map = parseMap(before);
    auto movements = parseMovements(after);
    for (auto movement : movements) {
//     printMap(map);
      applyMovement(map, movement);
    }

    uint64_t numGPS = 0;
    for (int i = 0; i < map.map.size(); i++) {
      for (int j = 0; j < map.map[i].size(); j++) {
        if (map.map[i][j] == Tile::LBOX) {
          numGPS += i*100 + j;
        }
      }
    }
    LOG(INFO) << "Number of GPS: " << numGPS;
    LOG(INFO) << "Done!";
    return 0;
}
