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

  auto customer1 = Customer(87, 65, 68, 1);

  int timeStep = (int)customer1.getBasicTimeStep();


  while (customer1.step(timeStep) != -1) {
      // Queue is not empty.
    
    std::string message = customer1.getMessage();
      if(message == "1R") {
        customer1.RemoteMode(timeStep);
         break;
        } else if (message == "exit") {
          return 0;
        } else if (message == "A") {
          customer1.autoMode();
        }

  };




  return 0;
}