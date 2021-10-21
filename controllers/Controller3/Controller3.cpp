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

  auto customer3 = Customer(87, 65, 68, 3);

  int timeStep = (int)customer3.getBasicTimeStep();

  auto receiver = static_cast<webots::Receiver*>(customer3.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(3);

  while (customer3.step(timeStep) != -1) {
    
      std::string message = customer3.getMessage();
      if(message == "3R") {
        customer3.RemoteMode(timeStep);
         break;
        } else if (message == "exit") {
          return 0;
        } else if (message == "A") {
          customer3.autoMode();
        }

  };




  return 0;
}