// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>

using namespace std;

struct Robot {
  int row;
  int col;
};

struct DirectionalKeypad : public KeyPad {
  DirectionalKeypad() {
    // initialize keypad
    keypad_ = {
        { '^', {0, 1} },
        { 'A', {0, 2} },
        { '<', {1, 0} },
        { 'v', {1, 1} },
        { '>', {1, 2} }
    };
    robot_.row = 0;
    robot_.col = 2;
  }
  ~DirectionalKeypad();
};

class KeyPad {
  public:
    virtual KeyPad() = 0;
    ~KeyPad() {}

    string moveRobotToChar(char c) {
      auto [targetRow, targetCol] = keypad_[c];
      int rowDiff = targetRow - robot_.row;
      int colDiff = targetCol - robot_.col;

      auto rowFirst = [&](int rdiff) {
        string s;
        if (rdiff > 0) s.append(rdiff, 'v');
        else if (rdiff < 0) s.append(-rdiff, '^');
        return s;
      };
      auto colFirst = [&](int cdiff) {
        string s;
        if (cdiff > 0) s.append(cdiff, '>');
        else if (cdiff < 0) s.append(-cdiff, '<');
        return s;
      };
      vector<string> results
      string sequence = makeColFirst(colDiff) + makeRowFirst(rowDiff) + 'A';
      results.push_back(sequence);
      sequence = makeRowFirst(rowDiff) + makeColFirst(colDiff) + 'A';
      return results;
    }

  private:
    Robot robot_;
    map<char, pair<int, int>> keypad_;
};

class NumericKeypad {
  public:
    NumericKeypad() {
      keypad_ = {
          { '7', {0, 0} },
          { '8', {0, 1} },
          { '9', {0, 2} },
          { '4', {1, 0} },
          { '5', {1, 1} },
          { '6', {1, 2} },
          { '1', {2, 0} },
          { '2', {2, 1} },
          { '3', {2, 2} },
          { '0', {3, 1} },
          { 'A', {3, 2} }
      };
      robot_.row = 3;
      robot_.col = 2;
    }
    ~NumericKeypad() {}

    string moveRobotToChar(char c) {
      auto [targetRow, targetCol] = keypad_[c];
      int rowDiff = targetRow - robot_.row;
      int colDiff = targetCol - robot_.col;
      string sequence;
      if (colDiff > 0) {
        sequence.append(colDiff, '>');
      } else if (colDiff < 0) {
        sequence.append(-colDiff, '<');
      }
      if (rowDiff > 0) {
        sequence.append(rowDiff, 'v');
      } else if (rowDiff < 0) {
        sequence.append(-rowDiff, '^');
      }
      sequence += 'A';
      robot_.row = targetRow;
      robot_.col = targetCol;
      return sequence;
    }

    string getSequence(string line) {
      string sequence;
      for (auto c : line) {
        sequence += moveRobotToChar(c);
      }
      return sequence;
    }

  private:
    Robot robot_;
    map<char, pair<int, int>> keypad_;
};

string getRecursiveSequence(const string& line, int layers) {
  if (layers == 0) {
    return line;
  }

  if (layers == 3) {
    NumericKeypad npad;
    string expanded = npad.getSequence(line);
    return getRecursiveSequence(expanded, layers - 1);
  }
  else {
    DirectionalKeypad dpad;
    string expanded = dpad.getSequence(line);
    return getRecursiveSequence(expanded, layers - 1);
  }
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    uint64_t total = 0;
    for (auto line : lines) {
      NumericKeypad npad;
      auto sequence = npad.getSequence(line);
      DirectionalKeypad dpad1;
      auto d1sequence = dpad1.getSequence(sequence);
      DirectionalKeypad dpad2;
      auto d2sequence = dpad2.getSequence(d1sequence);
      uint64_t numericCode = stoll(line.substr(0, line.size() - 1));
      uint64_t seqLen = d2sequence.size();
      LOG(INFO) << line;
      LOG(INFO) << sequence;
      LOG(INFO) << d1sequence;
      LOG(INFO) << d2sequence;
      LOG(INFO) << "Numeric code: " << numericCode << " Sequence length: " << seqLen;
      total += numericCode * seqLen;
    }

    LOG(INFO) << "Total complexity: " << total;
    LOG(INFO) << "Done!";
    return 0;
}
