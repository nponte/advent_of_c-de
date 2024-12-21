// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <regex>
#include <iostream>
#include <climits>
#include <cmath>

using namespace std;

struct Computer {
  uint64_t A;
  uint64_t B;
  uint64_t C;
  int inst_pointer;
};


struct Instruction {
  int opcode;
  int operand;
};

Computer parseComputer(vector<string> lines) {
    Computer computer;
    regex re("Register ([A,B,C]): ([0-9]+)");
    smatch match;
    for (auto line : lines) {
        if (regex_search(line, match, re)) {
            if (match[1] == "A") {
                computer.A = stoi(match[2]);
            } else if (match[1] == "B") {
                computer.B = stoi(match[2]);
            } else if (match[1] == "C") {
                computer.C = stoi(match[2]);
            }
        }
    }
    computer.inst_pointer = 0;
    return computer;
}

vector<Instruction> parseProgram(vector<string> lines) {
    std::regex pro("Program: (.*)");
    string program;
    smatch match;
    for (auto line : lines) {
        if (regex_search(line, match, pro)) {
          program = match[1];
        }
    }
 //   LOG(INFO) << "Program: " << program;
    std::vector<int> numbers;
    {
        std::istringstream iss(program);
        std::string token;
        while (std::getline(iss, token, ',')) {
            // After splitting by comma, we may still have spaces
            std::stringstream ss(token);
            int num;
            while (ss >> num) {
                if (num < 0 || num > 7) {
                    throw std::runtime_error("Invalid number in program: " + std::to_string(num));
                }
                numbers.push_back(num);
            }
        }
    }
    std::vector<Instruction> instructions;
    for (size_t i = 0; i < numbers.size(); i += 2) {
        int opcode = numbers[i];
        int operand = numbers[i + 1];
        instructions.push_back({opcode, operand});
    }
    return instructions;
}

uint64_t getComboValue(Computer& c, int operand) {
  if (operand <= 3) {
    return operand;
  } else if (operand == 4) {
    return c.A;
  } else if (operand == 5) {
    return c.B;
  } else if (operand == 6) {
    return c.C;
  }
  return 0;
}

void executeInstruction(Computer& c, Instruction i, string& output) {
  switch (i.opcode) {
    case 0: {
        // adv: A = floor(A / (2^(combo_value)))
        auto val = getComboValue(c, i.operand);
        // If val is large, pow(2, val) can be huge; 
        // For large exponents, consider overflow or large computations.
        // Assuming val is reasonable or using 64-bit ints:
        long long denom = (long long)1 << val; 
        // Using bit shift is efficient since it's 2^val.
        c.A = (c.A / denom);
        break;
    }
    case 1: {
        // bxl: B = B XOR literal_operand
        auto val = i.operand; // literal directly
        c.B = c.B ^ val;
        break;
    }
    case 2: {
        // bst: B = combo_value % 8
        auto val = getComboValue(c, i.operand);
        c.B = val % 8;
        break;
    }
    case 3: {
        // jnz: if A != 0, jump to literal_operand
        auto val = i.operand; // literal
        if (c.A != 0) {
            c.inst_pointer = val / 2; // Jump to the instruction
            return;
            // Important: After this, we do NOT increment by 2.
        }
        break;
    }
    case 4: {
        // bxc: B = B XOR C (operand is ignored)
        c.B = c.B ^ c.C;
        break;
    }
    case 5: {
        // out: output (combo_value % 8)
        auto val = getComboValue(c, i.operand) % 8;
        // Output val. If multiple outputs, separate by commas as per spec.
        // For simplicity, just print with a comma and no newline:
        output += std::to_string(val) + ",";
        break;
    }
    case 6: {
        // bdv: like adv but result stored in B
        auto val = getComboValue(c, i.operand);
        long long denom = (long long)1 << val;
        c.B = (c.A / denom);
        break;
    }
    case 7: {
        // cdv: like adv but result stored in C
        auto val = getComboValue(c, i.operand);
        long long denom = (long long)1 << val;
        c.C = (c.A / denom);
        break;
    }
    default:
        // Invalid opcode; handle error if needed
        break;
  }
  c.inst_pointer += 1;
}

string runProgram(Computer& c, vector<Instruction>& instructions) {
  string output;
  while (c.inst_pointer < instructions.size()) {
      auto i = instructions[c.inst_pointer];
      executeInstruction(c, i, output);
  }
  return output;
}

bool compareStr(string a, string b) {
  int offset = b.size() - a.size();
  for (int i = 0; i < a.size(); i++) {
    if (a[i] != b[i+offset]) {
      return false;
    }
  }
  return true;
}

uint64_t recursive(uint64_t testVal, vector<string> lines, string program) {
 // LOG(INFO) << "Testing A = " << testVal;
  auto instructions = parseProgram(lines);
  uint64_t r;
  for (int i = 0; i < 8; i++ ) {
    auto regA = testVal + i;
    auto computer = parseComputer(lines);
    computer.A = regA;
    string output = runProgram(computer, instructions);
    LOG(INFO) << "Output: " << output << " for A = " << regA << " program: " << program;
    if (compareStr(output, program)) {
      if (output.size() == program.size()) {
        LOG(INFO) << "Found A = " << testVal + i;
        return testVal + i;
      } else {
        r = recursive((testVal + i) * 8, lines, program);
        if (r != 0) {
          return r;
        }
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    std::regex pro("Program: (.*)");
    string program;
    smatch match;
    for (auto line : lines) {
        if (regex_search(line, match, pro)) {
          program = match[1];
        }
    }
    program += ",";

    auto solution = recursive(0, lines, program);

    LOG(INFO) << "A = " << solution;
    LOG(INFO) << "Done!";
    return 0;
}
