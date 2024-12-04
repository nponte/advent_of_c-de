// nponte

#ifndef DATA_H
#define DATA_H

#include <string>

using namespace std;

class Data {
public:
    explicit Data(const std::string& path) : filePath_(path) {}
    ~Data() {}

    vector<string> readLines();
    void print();
private:
    string filePath_;
};

#endif // DATA_H
