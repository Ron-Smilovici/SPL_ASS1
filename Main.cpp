#include "Restaurant.h"
#include <iostream>

using namespace std;

Restaurant* backup = nullptr;

int main(int argc, char** argv) {
    //the next four lines are for deletion before submission (until the if statement)
    std::ifstream in("/home/alon-pc/input_example2.txt");
    std::cin.rdbuf(in.rdbuf());
    std::ofstream out("/home/alon-pc/output_example2.txt");
    std::cout.rdbuf(out.rdbuf());
    if (argc != 2) {
        std::cout << "usage: rest <config_path>" << std::endl;
        return 0;
    }

    string configurationFile = argv[1];
    Restaurant rest(configurationFile);
    rest.start();
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    return 0;
}