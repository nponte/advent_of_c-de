// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>
#include <sstream>
#include <deque>

string getLowestEdge(std::map<string, vector<string>> &Graph) {
  std::optional<int> lowest_weight;
  string lowest_edge;
  for (auto it = Graph.begin(); it != Graph.end(); ++it) {
    if (!lowest_weight.has_value() || it->second.size() < lowest_weight.value()) {
      lowest_weight = it->second.size();
      lowest_edge = it->first;
    }
  }
  LOG(INFO) << "Lowest edge: " << lowest_edge;
  return lowest_edge;
}

using namespace std;

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    int i = 0;
    std::map<string, vector<string>> map;
    while (true) {
      auto line = lines[i++];
      LOG(INFO) << line;
      if (line == "") {
        break;
      }
      auto num1 = line.substr(0, line.find("|"));
      auto num2 = line.substr(line.find("|") + 1);
      map[num2].push_back(num1); 
    }
    // print out map
    for (auto it = map.begin(); it != map.end(); ++it) {
      string s = it->first + ':';
      for (auto val : it->second) {
        s += val + ',';
      }
      LOG(INFO) << s;
    }

    vector<string> updatesToFix;
    while (i < lines.size()) {
      auto line = lines[i++];
      LOG(INFO) << "Checking line " << line;
      std::stringstream ss(line);
      string linesofar;
      string token;
      bool good = true;
      while(std::getline(ss, token, ',')) {
        linesofar += token + ",";
        for (auto valid : map[token]) {
          if ((linesofar.find(valid) == string::npos) && (line.find(valid) != string::npos)) {
            //LOG(INFO) << "Bad line " << valid << " not found in " << linesofar;
            good = false;
          }
        }
      }
      if (!good) {
        LOG(INFO) << "Good line " << line;
        updatesToFix.push_back(line);
      }
    }

    int total = 0;
    for (auto line : updatesToFix) {
      std::map<string, vector<string>> Graph;
      LOG(INFO) << line;
      std::stringstream ss(line);
      string token;
      while (std::getline(ss, token, ',')) {
        vector<string> nums;
        for (string valid : map[token]) {
          if (line.find(valid) != string::npos) {
            nums.push_back(valid);
          }
        }
        Graph[token] = nums;
      }

      vector<int> fixedNums;
      while (!Graph.empty()) {
        string num = getLowestEdge(Graph);
        fixedNums.push_back(stoi(num));
        for (auto it = Graph.begin(); it != Graph.end(); ++it) {
          vector<string> nums;
          for (string q : it->second) {
            if (q != num) {
              nums.push_back(q);
            }
          }
          it->second = nums;
        }
        for (auto it = Graph.begin(); it != Graph.end(); ++it) {
          string s = it->first + ':';
          for (auto val : it->second) {
            s += val + ',';
          }
          LOG(INFO) << s;
        }
        Graph.erase(num);
        LOG(INFO) << "Erased " << num;
      }
      total += fixedNums[(fixedNums.size() - 1)/2];
    }
    LOG(INFO) << "Total: " << total;
    LOG(INFO) << "Done!";
    return 0;
}
