// nponte

#include <vector>
#include <glog/logging.h>
#include "Data.h"

using namespace std;

int main(int argc, char *argv[]) {
    // read data
    auto data = Data(argv[1]);
    data.print();
    auto lines = data.readLines();

    LOG(INFO) << "Done!";
    return 0;
}
