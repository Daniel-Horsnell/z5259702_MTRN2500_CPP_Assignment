#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

int main() {
    std::ifstream ifst{};
    std::string str {};

    ifst.open("Starting.csv", std::ios::in);
    if (!ifst) {
        std::cout << "File not found" << std::endl;
    }
    return 0;
}
