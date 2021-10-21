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

  auto customer = Customer(87, 65, 68);

  int timeStep = (int)customer.getBasicTimeStep();

  auto receiver = static_cast<webots::Receiver*>(customer.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(2);

  while (customer.step(timeStep) != -1) {
      // Queue is not empty.

      if (receiver->getQueueLength() > 0) {
          auto message =
              static_cast<std::string>((static_cast<const char*>(receiver->getData())));
          receiver->nextPacket();  // Pops queue.
          std::cout << "I have recevied: " << message << std::endl;
          if(message == "2R") {
            customer.RemoteMode(timeStep);
            break;
          } else if (message == "exit") {
            return 0;
          } else if (message == "A") {
            customer.autoMode();
          }
      }

  };




  return 0;
}
