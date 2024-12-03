// nponte

#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <glog/logging.h>
#include <algorithm>

using namespace std;


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

    vector<vector<int>> reports;
    // loop through vector and split by space
    for (int i = 0; i < data.size(); i++) {
        vector<int> report;

        // split by space
        // use sstream to split by space
        stringstream ss(data[i]);
        string token;
        while (getline(ss, token, ' ')) {
            report.push_back(stoi(token));
        }
        LOG(INFO) << "pushed report";
        reports.push_back(report);
    }

    uint64_t passed = 0;
    for (vector<int> report : reports) {
      bool pass = false;
      for (int direction : {-1, 1}) {
        int errors = 0;
        for (int i = 1; i < report.size(); i++) {
          bool dpass = true;
          if (direction == 1) {
            if (report[i] < report[i-1]) {
              if (errors == 1) {
                dpass = false;
                break;
              } else {
                errors++;
              }
            }
          } else {
            if (report[i] > report[i-1]) {
              if (errors == 1) {
                dpass = false;
                break;
              } else {
                errors++;
              }
            }
          }

          if (abs(report[i] - report[i-1]) < 1) {
            if (errors == 1) {
              dpass = false;
              break;
            } else {
              errors++;
            }
          }
          if (abs(report[i] - report[i-1]) > 3) {
            if (errors == 1) {
              dpass = false;
              break;
            } else {
              errors++;
            }
          }

          pass |= dpass;
        }
      }
      if (pass) {
        passed++;
      }
    }

    LOG(INFO) << "Passed: " << passed;

    cout << "Done." << endl;
    return 0;
}
