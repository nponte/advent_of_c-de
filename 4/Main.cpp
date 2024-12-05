// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <memory>
#include <algorithm>

using namespace std;


bool check_x_mas(std::vector<std::string>& data, int i, int j) {
  if (i - 1 < 0 || i + 1 >= data.size() || j - 1 < 0 || j + 1 >= data[i].size()) {
    return false;
  }
  string word;
  word += data[i-1][j-1];
  word += data[i-1][j+1];
  word += data[i+1][j-1];
  word += data[i+1][j+1];

  // make sure word is 2 M and two S
  if (count(word.begin(), word.end(), 'M') != 2 || count(word.begin(), word.end(), 'S') != 2) {
    return false;
  }
  if ((word.find("MM") != string::npos) ^ (word.find("SS") != string::npos)) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    auto word = "XMAS";
    int count = 0;
    for (int row = 0; row < lines.size();  row++) {
      int col = 0;
      while (col < lines[row].size()) {
        if (lines[row][col] == 'A') {
         count += check_x_mas(lines, row, col);
        }
        col++;
      }
    }

    LOG(INFO) << "Count: " << count;
    LOG(INFO) << "Done!";
    return 0;
}
