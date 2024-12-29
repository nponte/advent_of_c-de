// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <set>
#include <algorithm>
#include <map>

using namespace std;

class Computer {
public:
  Computer() : key("") {}
  Computer(string key) : key(key) {}

  bool operator<(const Computer& other) const {
      return key < other.key;
  }
  bool operator==(const Computer& other) const {
      return key == other.key;
  }

  string key;
};

map<Computer, set<Computer>> connections;

pair<string, string> getComputers(string line) {
  string one = line.substr(0, 2);
  string two = line.substr(3, 2);
  return { one, two };
}

vector<Computer> findFullyConnected(Computer candidate, set<Computer> leftComputers, vector<Computer> found) {
  for (auto c : found) {
    if (connections[c].find(candidate) == connections[c].end()) {
      found.pop_back();
      return found;
    }
  }
  leftComputers.erase(candidate);
  found.push_back(candidate);

  vector<Computer> largest;
  for (auto c : leftComputers) {
    auto newFound = findFullyConnected(c, leftComputers, found);
    if (newFound.size() > found.size()) {
      found = newFound;
    }
  }
  return found;
}

string getPassword(vector<Computer> computers) {
  // sort the keys
  vector<string> keys;
  for (auto c : computers) {
    keys.push_back(c.key);
  }
  sort(keys.begin(), keys.end());
  string password = "";
  for (auto key : keys) {
    password += key + ",";
  }
  password.pop_back();
  return password;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    map<string, Computer> computerMap;
    vector<string> computers;
    for (auto line : lines) {
      auto [ a, b ] = getComputers(line);
      if (computerMap.find(a) == computerMap.end()) {
        computers.push_back(a);
        computerMap[a] = Computer(a);
      }
      if (computerMap.find(b) == computerMap.end()) {
        computers.push_back(b);
        computerMap[b] = Computer(b);
      }
      connections[computerMap[a]].insert(computerMap[b]);
      connections[computerMap[b]].insert(computerMap[a]);
    }
    LOG(INFO) << "Computers: " << computers.size();


    for (auto computer : computers) {
      vector<Computer> largestSet;
      auto result = findFullyConnected(computerMap[computer], connections[computerMap[computer]], largestSet);
      auto pass = getPassword(result);
      LOG(INFO) << pass;
    }

    LOG(INFO) << "Done!";
    return 0;
}
