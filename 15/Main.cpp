// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <iostream>

using namespace std;
enum class Direction { UP, DOWN, LEFT, RIGHT };
enum class Tile { EMPTY, WALL, BOX, ROBOT };
struct Map {
  vector<vector<Tile>> map;
  pair<int, int> robot;
};

Map parseMap(vector<string> lines) {
  Map map;
  for (int i = 0; i < lines.size(); i++) {
    vector<Tile> row;
    for (int j = 0; j < lines[i].size(); j++) {
      auto c = lines[i][j];
      if (c == '.') {
        row.push_back(Tile::EMPTY);
      } else if (c == '#') {
        row.push_back(Tile::WALL);
      } else if (c == 'O') {
        row.push_back(Tile::BOX);
      } else if (c == '@') {
        map.robot = {i, j};
        row.push_back(Tile::ROBOT);
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
      } else if (tile == Tile::BOX) {
        cout << "O";
      } else if (tile == Tile::ROBOT) {
        cout << "@";
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

  // Robot attempts to push boxes, if the action would case the robot or a box to move int oa wall nothing moves including the robot
  bool foundSpace = false;
  auto pos = make_pair(newRobot.first + movVec.first, newRobot.second + movVec.second);
  while (true) {
    if (map.map[pos.first][pos.second] == Tile::WALL) {
      break;
    } else if (map.map[pos.first][pos.second] == Tile::EMPTY) {
      foundSpace = true;
      break;
    } 
    pos = make_pair(pos.first + movVec.first, pos.second + movVec.second);
  }
  if (foundSpace) {
    map.map[map.robot.first][map.robot.second] = Tile::EMPTY;
    map.robot = make_pair(map.robot.first + movVec.first, map.robot.second + movVec.second);
    map.map[map.robot.first][map.robot.second] = Tile::ROBOT;
    map.map[pos.first][pos.second] = Tile::BOX;
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
      applyMovement(map, movement);
    }

    uint64_t numGPS = 0;
    for (int i = 0; i < map.map.size(); i++) {
      for (int j = 0; j < map.map[i].size(); j++) {
        if (map.map[i][j] == Tile::BOX) {
          numGPS += i*100 + j;
        }
      }
    }
    LOG(INFO) << "Number of GPS: " << numGPS;
    LOG(INFO) << "Done!";
    return 0;
}
