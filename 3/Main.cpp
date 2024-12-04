// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <regex>

using namespace std;

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();

    auto lines = data.readLines();
    int count = 0;
    bool enabled = true;
    for (string line : lines) {
      LOG(INFO) << line;
      std::regex pattern(R"(mul\(([0-9]{1,3}),([0-9]{1,3})\)|(do\(\))|(don't\(\)))");
      auto mul_begin = std::sregex_iterator(line.begin(), line.end(), pattern);
      auto mul_end = std::sregex_iterator();
      for (std::sregex_iterator i = mul_begin; i != mul_end; ++i) {
        std::smatch match = *i;
        if (match[0].str() == "do()") {
          enabled = true;
        } else if (match[0].str() == "don't()") {
          enabled = false;
        } else {
          int num1 = std::stoi(match[1].str());
          int num2 = std::stoi(match[2].str());
          if (enabled) {
            count += num1 * num2;
          }
        }
      }
    }

    LOG(INFO) << "Count: " << count;
    LOG(INFO) << "Done!";
    return 0;
}
