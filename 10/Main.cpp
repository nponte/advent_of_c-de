// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <queue>
#include <set>

using namespace std;

uint64_t trailHeadScore(uint64_t row, uint64_t col, vector<string> lines) {
    uint64_t score = 0;
    queue<pair<uint64_t, uint64_t>> q;
    q.push({row, col});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (lines[r][c] == '9') {
            score++;
            continue;
        }
        for (pair<uint64_t, uint64_t> p : {std::make_pair(r + 1, c), std::make_pair(r - 1, c), std::make_pair(r, c + 1), std::make_pair(r, c - 1)}) {
          uint64_t nr = p.first;
          uint64_t nc = p.second;
          if (nr < 0 || nr >= lines.size() || nc < 0 || nc >= lines[nr].size()) {
              continue;
          }
          if ((lines[r][c] - '0') == (lines[nr][nc] - '0' - 1)) {
              q.push({nr, nc});
          }
        }
    }
    LOG(INFO) << "Score: " << score;
    return score;
}
int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    uint64_t sum = 0;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == '0') {
              sum += trailHeadScore(i, j, lines);
            }
        }
    }
    LOG(INFO) << "Sum of Scores: " << sum;

    LOG(INFO) << "Done!";
    return 0;
}
