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

  auto customer4 = Customer(87, 65, 68, 4);

  int timeStep = (int)customer4.getBasicTimeStep();

  auto receiver = static_cast<webots::Receiver*>(customer4.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(4);

  while (customer4.step(timeStep) != -1) {
      
      std::string message = customer4.getMessage();
      if(message == "4R") {
        customer4.RemoteMode(timeStep);
         break;
        } else if (message == "exit") {
          return 0;
        } else if (message == "A") {
          customer4.autoMode();
        }

  };




  return 0;
}