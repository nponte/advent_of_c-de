// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"
#include <map>
#include <regex>
#include <queue>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <fstream>

using namespace std;

enum class Operation { XOR, OR, AND };

int instructionId = 0;

struct Instruction {
  int id;
  Operation operation;
  string register1;
  string register2;
  string outputRegister;
};

string operationToString(Operation op) {
    switch (op) {
        case Operation::AND:
            return "AND";
        case Operation::OR:
            return "OR";
        case Operation::XOR:
            return "XOR";
        default:
            return "UNKNOWN";
   }
}

void generateGraphviz(const vector<Instruction>& instructions) {
    // Initialize the DOT graph
    stringstream ss;
    ss << "digraph G {\n";
    ss << "    rankdir=LR;\n"; // Left to Right layout
    ss << "    node [fontname=\"Arial\"];\n\n";

    // Sets to store unique registers and instructions
    unordered_set<string> registers;
    unordered_set<int> instructionIds;

    // Collect all unique registers and instruction IDs
    for (const auto& instr : instructions) {
        registers.insert(instr.register1);
        registers.insert(instr.register2);
        registers.insert(instr.outputRegister);
        instructionIds.insert(instr.id);
    }

    // Define styles for register nodes
    ss << "    // Register Nodes\n";
    for (const auto& reg : registers) {
        ss << "    \"" << reg << "\" [shape=box, style=filled, color=lightblue];\n";
    }
    ss << "\n";

    // Define styles for instruction nodes
    ss << "    // Instruction Nodes\n";
    for (const auto& instr : instructions) {
        string instrName = "instr" + to_string(instr.id);
        ss << "    \"" << instrName << "\" [shape=ellipse, style=filled, color=lightgreen, label=\"" 
           << operationToString(instr.operation) << "\\nID: " << instr.id << "\"];\n";
    }
    ss << "\n";

    // Define edges from registers to instructions and instructions to output registers
    ss << "    // Edges\n";
    for (const auto& instr : instructions) {
        string instrName = "instr" + to_string(instr.id);
        // Edge from register1 to instruction
        ss << "    \"" << instr.register1 << "\" -> \"" << instrName << "\";\n";
        // Edge from register2 to instruction
        ss << "    \"" << instr.register2 << "\" -> \"" << instrName << "\";\n";
        // Edge from instruction to outputRegister
        ss << "    \"" << instrName << "\" -> \"" << instr.outputRegister << "\";\n";
    }

    // Close the DOT graph
    ss << "}\n";

    // Write to File
    ofstream out("graph.dot");
    out << ss.str();
    out.close();
}

void printRegisters(map<string, int> registers) {
  for (auto [key, value] : registers) {
    LOG(INFO) << "Register: " << key << " Value: " << value;
  }
}

void printInstructions(vector<Instruction> instructions) {
  for (auto instruction : instructions) {
    LOG(INFO) << "Instruction: " << instruction.id << " " << instruction.register1 << " " << (instruction.operation == Operation::AND ? "AND" : instruction.operation == Operation::OR ? "OR" : "XOR") << " " << instruction.register2 << " -> " << instruction.outputRegister;
  }
}

map<string, int> parseRegister(vector<string> lines) {
  map<string, int> registers;
  regex re("([a-z0-9]+): (0|1)");
  smatch match;
  for (auto line : lines) {
    if (regex_search(line, match, re)) {
      registers[match[1]] = stoi(match[2]);
    }
  }
  return registers;
}

vector<Instruction> parseInstructions(vector<string> instructions) {
  vector<Instruction> parsedInstructions;
  regex re("([a-z0-9]+) (AND|OR|XOR) ([a-z0-9]+) -> ([a-z0-9]+)");
  smatch match;
  for (auto line : instructions) {
    if (regex_search(line, match, re)) {
      Instruction instruction;
      instruction.id = instructionId++;
      instruction.register1 = match[1];
      instruction.operation = match[2] == "AND" ? Operation::AND : match[2] == "OR" ? Operation::OR : Operation::XOR;
      instruction.register2 = match[3];
      instruction.outputRegister = match[4];
      parsedInstructions.push_back(instruction);
    }
  }
  return parsedInstructions;
}

void executeInstruction(Instruction instruction, map<string, int>& registers) {
  int value1 = registers[instruction.register1];
  int value2 = registers[instruction.register2];
  int result = 0;
  switch (instruction.operation) {
    case Operation::AND:
      result = value1 & value2;
      break;
    case Operation::OR:
      result = value1 | value2;
      break;
    case Operation::XOR:
      result = value1 ^ value2;
      break;
  }
  registers[instruction.outputRegister] = result;
}

bool areInstructionsExecutable(queue<Instruction> instructions, const map<string, int>& registers) {
  while (!instructions.empty()) {
    auto instruction = instructions.front();
    if (registers.find(instruction.register1) != registers.end() && registers.find(instruction.register2) != registers.end()) {
      return true;
    }
    instructions.pop();
  }
  return false;
}

map<string, int> executeInstructionsInOrder(vector<Instruction> instructions, map<string, int> registers) {
  queue<Instruction> instructionQueue;
  for (auto instruction : instructions) {
    instructionQueue.push(instruction);
  }
  while (!instructionQueue.empty()) {
    if (!areInstructionsExecutable(instructionQueue, registers)) {
      return {};
    }
    auto instruction = instructionQueue.front();
    instructionQueue.pop();
    if (registers.find(instruction.register1) != registers.end() && registers.find(instruction.register2) != registers.end()) {
      executeInstruction(instruction, registers);
    } else {
      instructionQueue.push(instruction);
    }
  }
  return registers;
}

uint64_t getRegisterValue(char registerName, map<string, int> registers) {
  vector<string> registerNames;
  for (auto [key, value] : registers) {
    if (key[0] == registerName) {
      registerNames.push_back(key);
    }
  }
  sort(registerNames.begin(), registerNames.end());
  string bits;
  for (int i = 0; i < registerNames.size(); i++) {
    bits = to_string(registers[registerNames[i]]) + bits;
  }
  return stoull(bits, nullptr, 2);
}

vector<Instruction> executeSwaps(vector<Instruction> instructions, vector<pair<string, string>> swaps) {
  for (auto& instruction : instructions) {
    for (auto [instruction1, instruction2] : swaps) {
      if (instruction.outputRegister == instruction1) {
        instruction.outputRegister = instruction2;
      } else if (instruction.outputRegister == instruction2) {
        instruction.outputRegister = instruction1;
      }
    }
  }
  return instructions;
}

vector<pair<Instruction, Instruction>> getInstructionsToSwap(vector<Instruction> instructions) {
  vector<pair<Instruction, Instruction>> pairs;
  for (int i = 0; i < instructions.size(); i++) {
    for (int j = i + 1; j < instructions.size(); j++) {
      pairs.push_back({instructions[i], instructions[j]});
    }
  }
  return pairs;
}

void generateCombinations(
  uint64_t desiredZValue,
  vector<pair<Instruction, Instruction>> pairs,
  vector<Instruction> instructions,
  map<string, int> registers,
  vector<pair<string, string>>& current
) {
  if (current.size() == 4) {
    instructions = executeSwaps(instructions, current);
    registers = executeInstructionsInOrder(instructions, registers);
    if (registers.empty()) {
      return;
    }
    auto value = getRegisterValue('z', registers);
    if (value == desiredZValue) {
      vector<string> names;
      for (auto [instruction1, instruction2] : current) {
        names.push_back(instruction1);
        names.push_back(instruction2);
      }
      sort(names.begin(), names.end());
      string result;
      for (auto name : names) {
        result += name + ",";
      }
      LOG(INFO) << "Result: " << result;
    }
    return;
  }
  for (int i = 0; i < pairs.size(); i++) {
    bool alreadyUsed = false;
    for (auto [instruction1, instruction2] : current) {
      if (instruction1 == pairs[i].first.outputRegister || instruction1 == pairs[i].second.outputRegister) {
        alreadyUsed = true;
      }
      if (instruction2 == pairs[i].first.outputRegister || instruction2 == pairs[i].second.outputRegister) {
        alreadyUsed = true;
      }
    }
    if (alreadyUsed) {
      continue;
    }
    current.push_back({pairs[i].first.outputRegister, pairs[i].second.outputRegister});
    generateCombinations(desiredZValue, pairs, instructions, registers, current);
    current.pop_back();
  }
}

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    int emptyLine = 0;
    for (int i = 0; i < lines.size(); i++) {
      if (lines[i].empty()) {
        emptyLine = i;
        break;
      }
    }
    vector<string> registerLines(lines.begin(), lines.begin() + emptyLine);
    auto registers = parseRegister(registerLines);
    LOG(INFO) << "Parsed registers: " << registers.size();
    vector<string> instructionLines(lines.begin() + emptyLine + 1, lines.end());
    auto parsedInstructions = parseInstructions(instructionLines);
    parsedInstructions = executeSwaps(parsedInstructions, {{"z12", "djg"}, {"z19", "sbg"}, {"z37", "dsd"}});
    generateGraphviz(parsedInstructions);
    LOG(INFO) << "Parsed instructions: " << parsedInstructions.size();
    
    uint64_t desiredZValue = getRegisterValue('y', registers) + getRegisterValue('x', registers);
    LOG(INFO) << "Desired Z value: " << desiredZValue;

    vector<pair<Instruction, Instruction>> pairs = getInstructionsToSwap(parsedInstructions);
    shuffle(pairs.begin(), pairs.end(), mt19937{random_device{}()});
    vector<pair<string, string>> currentSwaps = {{"z12", "djg"}, {"z19", "sbg"}};
    generateCombinations(desiredZValue, pairs, parsedInstructions, registers, currentSwaps);

    LOG(INFO) << "Done!";
    return 0;
}
