// nponte

#ifndef DATA_H
#define DATA_H

using namespace std;

class Data {
public:
    explicit Data(const std::string& path) : filePath_(path) {}
    ~Data() {}

    vector<std::string> readLines();
    void print();
private:
    std::string filePath_;
};

#endif // DATA_H
