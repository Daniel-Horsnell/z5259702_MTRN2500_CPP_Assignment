// File:          Main_Controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <iostream>
#include <webots/Keyboard.hpp>

// All the webots classes are defined in the "webots" namespace
using namespace webots;


void menu(webots::Robot& robot, int timeStep) {
  auto keyboard = robot.getKeyboard();
  keyboard->enable(timeStep);
  // Printing initial setup menu.
  std::cout << "This is a simulation for MTRN2500 Cafe." << std::endl;
  std::cout << "Press [I] to reprint the commands." << std::endl;
  std::cout << "Press [R] to remote control a robot." << std::endl;
  std::cout << "Press [A] to enter the auto mode." << std::endl;
  std::cout << "Press [Q] to quit all controllers." << std::endl;
  // Cooldown counter so it doesn't print the command out multiple times.
  auto i = 0;
  while (robot.step(timeStep) != -1) {
    auto key = keyboard->getKey();
    if (key != -1 && i > 10) {
    //setting cooldown to 0
    i = 0;
      switch(key) {
        // Case for pressing i or I.
        case 73 : case 65609 :
          std::cout << "This is a simulation for MTRN2500 Cafe.\n";
          std::cout << "Press [I] to reprint the commands.\n";
          std::cout << "Press [R] to remote control a robot.\n";
          std::cout << "Press [A] to enter the auto mode.\n";
          std::cout << "Press [Q] to quit all controllers." << std::endl;
          break;
        // Case for pressing r or R.
        case 82 : case 65618 :
          std::cout << "Entering remote control mode" << std::endl;
          //remoteControl();
          break;
        // Case for pressing a or A.
        case 65 : case 65601 :
          std::cout << "Entering auto mode." << std::endl;
          //Auto;
          break;
        // Case for pressing q or Q.
        case 81 : case 65617 :
          std::cout << "Quiting" << std::endl;
          break;
        default :
          std::cout << "Command not found." << std::endl;
          break;
      }
    }
    i++;
  };
  return;
}

int main(int argc, char **argv) {
  // create the Robot instance.
  auto robot = webots::Robot();

  // get the time step of the current world.
  auto const timeStep = robot.getBasicTimeStep();

    
    // }
  // }
  // making menu.
  menu(robot, timeStep);
    
  return 0;
}