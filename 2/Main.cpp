// nponte

#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <glog/logging.h>
#include <algorithm>
#include "Data.h"

using namespace std;

bool passReport(vector<int> report) {
    stringstream ss;
    ss << "Report: ";
    for (int i = 0; i < report.size(); i++) {
      ss << report[i] << " ";
    }
    LOG(INFO) << ss.str();
    int i = 0, j = 1, direction = 0;
    while (j < report.size()) {
      if (
        (report[j] == report[i]) ||
        (abs(report[j] - report[i]) > 3)
      ) {
        return false;
      }
      if (direction == 0) {
        if (report[j] > report[i]) {
          direction = 1;
        } else {
          direction = -1;
        }
      } else {
        if (
          (report[j] > report[i] && direction == -1) ||
          (report[j] < report[i] && direction == 1)
        ) {
          return false;
        }
      }
      i++;
      j++;
    }
  return true;
}
  
int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();

    auto lines = data.readLines();
    vector<vector<int>> reports;
    // loop through vector and split by space
    for (int i = 0; i < lines.size(); i++) {
        vector<int> report;

        // split by space
        // use sstream to split by space
        stringstream ss(lines[i]);
        string token;
        while (getline(ss, token, ' ')) {
            report.push_back(stoi(token));
        }
        reports.push_back(report);
    }

    uint64_t passed = 0;
    int row = 0;
    for (vector<int> report : reports) {
      LOG(INFO) << "Report " << row;
      row++;
      bool pass = false;
      pass |= passReport(report);
      for (int i = 0; i < report.size(); i++) {
        vector<int> singleBad;
        for (int j = 0; j < report.size(); j++) {
          if (i == j) {
            continue;
          }
          singleBad.push_back(report[j]);
        }
        pass |= passReport(singleBad);
      }
      if (pass) {
        passed++;
      }
    }

    LOG(INFO) << "Passed: " << passed;
    
    cout << "Done." << endl;
    return 0;
}
