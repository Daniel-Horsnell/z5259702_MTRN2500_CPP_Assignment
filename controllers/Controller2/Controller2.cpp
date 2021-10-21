// File:          Controller2.cpp
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

  auto customer2 = Customer(87, 65, 68, 2);

  int timeStep = (int)customer2.getBasicTimeStep();

  auto receiver = static_cast<webots::Receiver*>(customer2.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(2);

  while (customer2.step(timeStep) != -1) {
      // Queue is not empty.

      std::string message = customer2.getMessage();
      if(message == "2R") {
        customer2.RemoteMode(timeStep);
         break;
        } else if (message == "exit") {
          return 0;
        } else if (message == "A") {
          customer2.autoMode();
        }

  };




  return 0;
}
