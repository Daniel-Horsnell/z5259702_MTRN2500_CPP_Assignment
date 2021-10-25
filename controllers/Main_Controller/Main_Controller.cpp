// File:          Main_Controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Supervisor.hpp>
#include <iostream>
#include <webots/Keyboard.hpp>
#include "..\Helper.cpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

// From Tute.
std::ifstream read(std::string filename) {
    std::ifstream in{};
    
    in.open(filename, std::ios::in);
    if (!in) {
        std::cout << "Can not open file." << std::endl;
    }
    return in;
}

int main(int argc, char **argv) {
  // Reading orders in.
  
  auto input {read("../Order.csv")};

  std::string line;
  getline(input, line);
  std::vector <std::string> orders;
  while(getline(input, line)) {
    std::string temp1;
    temp1.push_back(line.at(0));
    std::string temp2 = line.erase(0,2);
    orders.push_back(temp1);
    orders.push_back(temp2);  
    }


  auto director = Director(orders);
  director.menu();
    
  return 0;
}

