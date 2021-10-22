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

// All the webots classes are defined in the "webots" namespace
using namespace webots;

int main(int argc, char **argv) {
  // create the Supervisor instance.
  auto director = Director();
  std::string filename {"../Starting.csv"};
  std::ifstream ifst{};
  std::string str {};

  ifst.open(filename, std::ios::in);
  if (!ifst) {
    std::cout << "Can not open file." << std::endl;
  }
  std::string line;
  getline(ifst, line);
  std::cout << line << std::endl;
  director.menu();
    
  return 0;
}