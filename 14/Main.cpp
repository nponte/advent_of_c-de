// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <regex>
#include <set>
#include <climits>
#include <iostream>

using namespace std;

struct Robot {
  pair<int, int> pos;
  pair<int, int> vel;
};

pair<int, int> space = {101, 103};

void moveRobot(Robot& robot) {
  robot.pos.first = (robot.pos.first + robot.vel.first + space.first) % space.first;
  robot.pos.second = (robot.pos.second + robot.vel.second + space.second) % space.second;
}

vector<Robot> parseRobots(vector<string> lines) {
  vector<Robot> robots;
  regex re("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");
  for (auto line : lines) {
    smatch match;
    if (regex_match(line, match, re)) {
      Robot robot;
      robot.pos.first = stoi(match[1]);
      robot.pos.second = stoi(match[2]);
      robot.vel.first = stoi(match[3]);
      robot.vel.second = stoi(match[4]);
      robots.push_back(robot);
    }
  }
  return robots;
}

void printGrid(vector<Robot> robots) {
  auto width = space.first;
  auto height = space.second;

  // Step 1: Initialize the grid with "false"
  std::vector<std::vector<bool>> grid(height, std::vector<bool>(width, false));

  // Step 2: Mark positions on the grid
  for (const auto& robot : robots) {
      int px = robot.pos.first;   // X-coordinate
      int py = robot.pos.second;  // Y-coordinate
      grid[py][px] = true;
  }

  // Step 3: Print the grid
  for (int row = 0; row < height; ++row) {
      for (int col = 0; col < width; ++col) {
          if (grid[row][col]) {
              std::cout << "#";
          } else {
              std::cout << ".";
          }
      }
      std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool isChristmasTree(vector<Robot> robots) {
  auto width = space.first;
  auto height = space.second;

  map<pair<int, int>, bool> grid;
  for (auto robot : robots) {
    grid[robot.pos] = true;
  }

  // Find 5 horizontal points
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width - 4; ++col) {
      if (
          grid[{col, row}] &&
          grid[{col + 1, row}] &&
          grid[{col + 2, row}] &&
          grid[{col + 3, row}] &&
          grid[{col + 4, row}]) {
        return true;
      }
    }
  }
  return false;
}

uint64_t safetyFactor(vector<Robot> robots) {
  uint64_t top_left = 0, top_right = 0, bottom_left = 0, bottom_right = 0;
  for (auto robot : robots) {
    if (robot.pos.first < space.first / 2 && robot.pos.second < space.second / 2) {
      top_left++;
    } else if (robot.pos.first < space.first / 2 && robot.pos.second > space.second / 2) {
      bottom_left++;
    } else if (robot.pos.first > space.first / 2 && robot.pos.second < space.second / 2) {
      top_right++;
    } else if (robot.pos.first > space.first / 2 && robot.pos.second > space.second / 2) {
      bottom_right++;
    }
  }
  return top_left * top_right * bottom_left * bottom_right;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    auto robots = parseRobots(lines);
    LOG(INFO) << "Robots: " << robots.size();

    auto maxSeconds = 100000;
    for (int i = 0; i < maxSeconds; i++) {
      for (auto& robot : robots) {
        moveRobot(robot);
      }
      if (isChristmasTree(robots)) {
        LOG(INFO) << "Seconds: " << i;
        printGrid(robots);
      }
    }

    LOG(INFO) << "Done!";
    return 0;
}
