// Print world in main(argc, argv)
// Compile: g++ -o hello hello.cpp
// Run: ./hello


#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <glog/logging.h>
#include <algorithm>

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

    // sort the locations
    locations1 = sortVector(locations1);
    locations2 = sortVector(locations2);

    uint64_t diff = 0;
    for (int i = 0; i < locations1.size(); i++) {
 //       LOG(INFO) << "location1: " << locations1[i] << " location2: " << locations2[i];
        diff += abs(locations1[i] - locations2[i]);
    }

    cout << "The difference is: " << diff << endl;
    cout << "Done." << endl;
    return 0;
}
