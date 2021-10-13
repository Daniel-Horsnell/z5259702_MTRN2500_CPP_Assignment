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

auto constexpr MAX_MOTOR_SPEED = 6.28;

// All the webots classes are defined in the "webots" namespace
using namespace webots;

// class Staff : public Robot {
//   public:
//     AutoMode()
// }

void AutoMode(webots::Robot& robot, int timeStep) {
  double velocity = 0.5*MAX_MOTOR_SPEED;
  int cooldown = 5;
  std::cout << "entering remote" << std::endl;
  
  auto leftMotor = static_cast<webots::Motor*>(robot.getMotor("left wheel motor"));
  auto rightMotor = static_cast<webots::Motor*>(robot.getMotor("right wheel motor"));

  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  leftMotor->setVelocity(0.0);
  rightMotor->setVelocity(0.0);

  auto keyboard = robot.getKeyboard();
  keyboard->enable(timeStep);
  
  while (robot.step(timeStep) != -1) {
    cooldown++;
    auto key = keyboard->getKey();
    if(key != -1) {
      switch(key) {
        // Move forward.
        case 315 :
          leftMotor->setVelocity(velocity);
          rightMotor->setVelocity(velocity);
          break;
        // Turn right.
        case 316 :
          leftMotor->setVelocity(velocity);
          rightMotor->setVelocity(-1*velocity);
          break;
        // Turn left.
        case 314 :
          leftMotor->setVelocity(-1*velocity);
          rightMotor->setVelocity(velocity);
          break;
        // Increase Speed.
        case 61 :
          if(velocity < MAX_MOTOR_SPEED && cooldown >= 5) {
            std::cout << "increasing speed" << std::endl;
            velocity += 0.1*MAX_MOTOR_SPEED;
            std::cout << velocity << std::endl;
            leftMotor->setVelocity(velocity);
            rightMotor->setVelocity(velocity);
            cooldown = 0;
          }
          break;
        // Decrease Speed.
        case 45 :
          if(velocity > 0 && cooldown >= 5) {
            std::cout << "decreasing speed" << std::endl;
            velocity -= 0.1*MAX_MOTOR_SPEED;
            std::cout << velocity << std::endl;
            leftMotor->setVelocity(velocity);
            rightMotor->setVelocity(velocity);
            cooldown = 0;
          }
          break;
        // Pause.
        case 32 :
          leftMotor->setVelocity(0);
          rightMotor->setVelocity(0);
          break;
        // Exit.
        case 65605 : case 69 :
          std::cout << "exit" << std::endl;
        default :
          break;
        
      }
    } 

  };
}

int main(int argc, char **argv) {

  auto robot = webots::Robot();

  std::cout << "starting" << std::endl;
  int timeStep = (int)robot.getBasicTimeStep();


  AutoMode(robot, timeStep);

  while (robot.step(timeStep) != -1) {

  };




  return 0;
}



