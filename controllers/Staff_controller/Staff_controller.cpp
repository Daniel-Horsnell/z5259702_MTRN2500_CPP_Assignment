// File:          Staff_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

#include <array>
#include <iostream>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Keyboard.hpp>
#include <webots/Emitter.hpp>
#include "..\Helper.cpp"
#include <webots/Receiver.hpp>
#include <webots/Supervisor.hpp>
#include <string>
#include <vector>
#include <fstream>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

int main(int argc, char **argv) {

  std::string file {"../Starting.csv"};
  std::ifstream in{};
  std::string entry;
  in.open(file, std::ios::in);
  if (!in) {
    std::cout << "Can not open file." << std::endl;
  }
  double money;
  getline(in, entry);
  while(getline(in,entry)) {
    if(entry[0] == '5') {
      int size = entry.length();
      std::string temp = entry.substr(2,size);
      money = std::stod(temp);
    }
  }

  std::string filename {"../Menu.csv"};
  std::ifstream input{};
  std::vector <std::string> menu;
  std::string line;
  input.open(filename, std::ios::in);
  if (!input) {
    std::cout << "Can not open file." << std::endl;
  }
  getline(input, line);
  while(std::getline(input, line)) {
    menu.push_back(line);
  }

  auto staff = Staff(315, 314, 316, 5, menu, money);

  int timeStep = (int)staff.getBasicTimeStep();



  while (staff.step(timeStep) != -1) {
      // Queue is not empty.
    std::string message = staff.getMessage();

    if(message == "5R") {
      staff.RemoteMode(timeStep);
      break;
    } else if (message == "exit") {
      return 0;
    } else if (message == "A") {
      staff.autoMode();
      staff.showAccount(money);
    }
      

  };




  return 0;
}



