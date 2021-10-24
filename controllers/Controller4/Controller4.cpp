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
    if(line[0] == '4') {
      int size = line.length();
      std::string temp = line.substr(2,size);
      money = std::stod(temp);
    }
  }

  auto customer4 = Customer(87, 65, 68, 4, money, -1.395, -0.64);

  int timeStep = (int)customer4.getBasicTimeStep();
  bool wasAuto = false;
  auto receiver = static_cast<webots::Receiver*>(customer4.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(4);

  while (customer4.step(timeStep) != -1) {
      
      std::string message = customer4.getMessage();
      if(message == "4R") {
        customer4.RemoteMode(timeStep);
         break;
        } else if (message == "exit") {
          break;
        } else if (message == "A") {
          customer4.autoMode();
          wasAuto = true;
        }

  };
  if (wasAuto) {
    customer4.showAccount();
  }



  return 0;
}