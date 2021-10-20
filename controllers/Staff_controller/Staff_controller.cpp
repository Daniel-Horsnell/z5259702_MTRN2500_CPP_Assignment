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

  auto staff = Staff(315, 314, 316);

  int timeStep = (int)staff.getBasicTimeStep();

  auto receiver = static_cast<webots::Receiver*>(staff.getReceiver("receiver"));
  receiver->enable(timeStep);
  receiver->setChannel(5);

  while (staff.step(timeStep) != -1) {
      // Queue is not empty.
      if (receiver->getQueueLength() > 0) {
          auto message =
              static_cast<std::string>((static_cast<const char*>(receiver->getData())));
          receiver->nextPacket();  // Pops queue.
          std::cout << "I have recevied: " << message << std::endl;
          if(message == "5R") {
            staff.RemoteMode(timeStep);
            break;
          }
      }
  };




  return 0;
}



