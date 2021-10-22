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

// All the webots classes are defined in the "webots" namespace
using namespace webots;

int main(int argc, char **argv) {

  auto staff = Staff(315, 314, 316, 5);

  int timeStep = (int)staff.getBasicTimeStep();



  while (staff.step(timeStep) != -1) {
      // Queue is not empty.
    std::string message = staff.getMessage();
    std::cout << message << std::endl;
    if(message == "5R") {
      std::cout << "made it baby." << std::endl;
      staff.RemoteMode(timeStep);
      break;
    } else if (message == "exit") {
      return 0;
    } else if (message == "A") {
      staff.autoMode();
    }
      

  };




  return 0;
}



