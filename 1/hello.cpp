// Print world in main(argc, argv)
// Compile: g++ -o hello hello.cpp
// Run: ./hello


#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <glog/logging.h>
#include <algorithm>
#include <map>

using namespace std;


std::vector<int> sortVector(const std::vector<int>& input) {
    std::vector<int> sorted = input;
    sort(sorted.begin(), sorted.end());
    for (int num : sorted) {
        LOG(INFO) << "sorted: " << num;
    }
    return sorted;
}

vector<string> readFile(string filename) {
    ifstream file(filename);
    string line;
    vector <string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

int main(int argc, char *argv[]) {
    // read from fiile 
    // ifstream file(argv[1]);
    vector<string> data = readFile(argv[1]);
    LOG(INFO) << "read file";

    vector<int> locations1, locations2;
    // loop through vector and split by space
    for (int i = 0; i < data.size(); i++) {
        locations1.push_back(stoi(data[i].substr(0, data[i].find(" "))));
        locations2.push_back(stoi(data[i].substr(data[i].find(" ") + 1)));
    }

    map<int, int> count1, count2;
    for (auto i : locations1) {
      if (count1.find(i) == count1.end()) {
        count1[i] = 1;
      } else {
        count1[i]++;
      }
    }
    for (auto j : locations2) {
      if (count2.find(j) == count2.end()) {
        count2[j] = 1;
      } else {
        count2[j]++;
      }
    }
    for (auto i : count1) {
      LOG(INFO) << "num: " << i.first << " count: " << i.second;
    }
    for (auto i : count2) {
      LOG(INFO) << "num: " << i.first << " count: " << i.second;
    }

    uint64_t diff = 0;
    for (auto i : locations1) {
      if (count2.find(i) != count2.end()) {
        diff += i * count2[i];
      }
    }

    cout << "The difference is: " << diff << endl;
    cout << "Done." << endl;
    return 0;
}
