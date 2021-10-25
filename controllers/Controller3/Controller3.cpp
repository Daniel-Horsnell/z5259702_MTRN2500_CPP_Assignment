// File:          customer_controller.cpp
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

// All the webots classes are defined in the "webots" namespace
using namespace webots;

int main(int argc, char **argv) {

  std::string filename {"../Starting.csv"};
  std::ifstream input{};
  std::vector <std::string> menu;
  std::string line;
  input.open(filename, std::ios::in);
  if (!input) {
    std::cout << "Can not open file." << std::endl;
  }
  double money;
  getline(input, line);
  while(getline(input,line)) {
    if(line[0] == '3') {
      int size = line.length();
      std::string temp = line.substr(2,size);
      money = std::stod(temp);
    }
  }

  auto customer3 = Customer(87, 65, 68, 3, money, -1.395, -0.14);
  bool wasAuto = false;
  int timeStep = (int)customer3.getBasicTimeStep();

  while (customer3.step(timeStep) != -1) {
    
      std::string message = customer3.getMessage();
      if(message == "3R") {
        customer3.RemoteMode();
         break;
        } else if (message == "exit") {
          break;
        } else if (message == "A") {
          customer3.autoMode();
          wasAuto = true;
        }

  };
  if (wasAuto) {
    customer3.showAccount();
  }




  return 0;
}