// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include "regex"
#include <queue>
#include <set>
#include <cmath>

using namespace std;

struct Game {
  pair<uint64_t, uint64_t> prize;
  pair<uint64_t, uint64_t> buttonA;
  pair<uint64_t, uint64_t> buttonB;
};

vector<Game> parseGames(vector<string> lines) {
  vector<Game> games;
  Game game;
  std::regex buttonA(R"(Button A: X\+(\d+), Y\+(\d+))");
  std::regex buttonB(R"(Button B: X\+(\d+), Y\+(\d+))");
  std::regex prize(R"(Prize: X=(\d+), Y=(\d+))");
  for (int i = 0; i < lines.size(); i++) {
    auto line = lines[i];
    smatch matches;
    if (regex_match(line, matches, buttonA)) {
      game.buttonA = make_pair(stoll(matches[1]), stoll(matches[2]));
    } else if (regex_match(line, matches, buttonB)) {
      game.buttonB = make_pair(stoll(matches[1]), stoll(matches[2]));
    } else if (regex_match(line, matches, prize)) {
      game.prize = {stoll(matches[1]) + 10000000000000LL, stoll(matches[2]) + 10000000000000LL};
      games.push_back(game);
      Game game;
    }
  }
  for (auto game : games) {
    LOG(INFO) << "Button A: " << game.buttonA.first << ", " << game.buttonA.second;
    LOG(INFO) << "Button B: " << game.buttonB.first << ", " << game.buttonB.second;
    LOG(INFO) << "Prize: " << game.prize.first << ", " << game.prize.second;
  }
  LOG(INFO) << "Games: " << games.size();
  return games;
}

uint64_t fewestTokens(Game g) {
  auto a_x = g.buttonA.first;
  auto a_y = g.buttonA.second;
  auto b_x = g.buttonB.first;
  auto b_y = g.buttonB.second;
  auto prize_x = g.prize.first;
  auto prize_y = g.prize.second;
  int64_t det = a_x * b_y - a_y * b_x;
  if (det == 0)
    return 0;

  int64_t a_num = prize_x * b_y - prize_y * b_x; // Numerator for 'a'
  int64_t b_num = a_x * prize_y - a_y * prize_x; // Numerator for 'b'
  
  if (a_num % det == 0 && b_num % det == 0) {
      int64_t a_int = a_num / det;
      int64_t b_int = b_num / det;
  
      if (a_int >= 0 && b_int >= 0) {
          return a_int * 3 + b_int;
      }
  }
//  long double a =
//      (prize_x * (long double)b_y - prize_y * (long double)b_x) / det;
//  long double b =
//      (a_x * (long double)prize_y - a_y * (long double)prize_x) / det;
//
//  if (a >= 0 && b >= 0 && abs(a - roundl(a)) < 1e-10 &&
//      abs(b - roundl(b)) < 1e-10) {
//    int64_t a_int = roundl(a);
//    int64_t b_int = roundl(b);
//    return a_int * 3 + b_int;
//  }
  return 0;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    auto games = parseGames(lines);

    uint64_t total = 0;
    for (auto game : games) {
      total += fewestTokens(game);
    }
    LOG(INFO) << "Total: " << total;
    LOG(INFO) << "Done!";
    return 0;
}
