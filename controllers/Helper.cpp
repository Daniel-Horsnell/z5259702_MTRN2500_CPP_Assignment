#include <array>
#include <iostream>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Keyboard.hpp>
#include <webots/Emitter.hpp>
#include <webots/Receiver.hpp>
#include <webots/Supervisor.hpp>
#include <string>

auto constexpr MAX_MOTOR_SPEED = 6.28;

using namespace webots;

class Person : public webots::Robot {
    public:
    Person(int forward, int left, int right) : Robot() {
        mForward = forward;
        mRight = right;
        mLeft = left;
        mTimeStep = getBasicTimeStep();
        mKeyboard = getKeyboard();
        mEmitter = getEmitter("emitter");
        mReceiver = getReceiver("receiver");
    }
    auto exit();
    
    void RemoteMode(int timeStep);
    private:
    int mForward;
    int mRight;
    int mLeft;
    int mTimeStep;
    webots::Keyboard *mKeyboard;
    webots::Emitter *mEmitter;
    webots::Receiver *mReceiver;

};

class Staff : public Person {
  public:
  Staff(int forward, int left, int right) : Person(forward, left, right) {}
};

class Customer : public Person {
  public:
  Customer(int forward, int left, int right) : Person(forward, left, right) {}
};

auto Person::exit() {
    mEmitter->setChannel(0);
    std::string message = "exit";
    if (mEmitter->send(message.c_str(), message.size() + 1)) {
        std::cout << "Exiting" << std::endl;
        return;
    }
    std::cout << "Failed to send a message :(" << std::endl;
}


class Director : public webots::Supervisor {
  public:
  Director() : Supervisor() {
    mTimeStep = getBasicTimeStep();
    mKeyboard = getKeyboard();
    mEmitter = getEmitter("emitter");
    mReceiver = getReceiver("receiver");
  }
  void menu();
  void remoteControl();
  auto setRobotMode(std::string mode);
  void idle();
  private:
  int mTimeStep;
  webots::Keyboard *mKeyboard;
  webots::Emitter *mEmitter;
  webots::Receiver *mReceiver;
};

void Director::idle() {
  mReceiver->enable(mTimeStep);
  mReceiver->setChannel(0);
  while(step(mTimeStep) != -1) {
    if (mReceiver->getQueueLength() > 0) {
      auto message = static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
        mReceiver->nextPacket();  // Pops queue.
        std::cout << "I have recevied: " << message << std::endl;
        return;
      }
  };

}
auto Director::setRobotMode(std::string mode) {
  int channel = mode.at(0);
  channel = channel - 48;
  mEmitter->setChannel(channel);
    if (mEmitter->send(mode.c_str(), mode.size() + 1)) {
        std::cout << "Sent a message :)" << std::endl;
        idle();
        return;
    }
    std::cout << "Failed to send a message :(" << std::endl;
}

void Director::remoteControl() {
  std::cout << "Director: Please select the robot to control remotely:" << std::endl;
  std::cout << "Director: Press [1] to control the Purple Robot (Customer1)." << std::endl;
  std::cout << "Director: Press [2] to control the White Robot (Customer2)." << std::endl;
  std::cout << "Director: Press [3] to control the Gold Robot (Customer3)." << std::endl;
  std::cout << "Director: Press [4] to control the Green Robot (Customer4)." << std::endl;
  std::cout << "Director: Press [5] to control the Black Robot (Staff)." << std::endl;

  mKeyboard->enable(mTimeStep); 
  // Cooldown timer.
  auto i = 0;
  while (step(mTimeStep) != -1) {
    i++;
    auto key = mKeyboard->getKey();
    if (key != -1 && i > 5) {
    //setting cooldown to 0

      switch(key) {
        // Pressing 1.
        case 49 :
          setRobotMode("1R");
          break;
        // Pressing 2.
        case 50 :
          setRobotMode("2R");
          break;
        // Pressing 3.
        case 51 :
          setRobotMode("3R");
          break;
        // Pressing 4.
        case 52 :
          setRobotMode("4R");
          break;
        case 53 :
          setRobotMode("5R");
          break;
        default :
          menu();
          break;
      }
      return;
    }
  };
  return;
}

void Director::menu() {
  mKeyboard->enable(mTimeStep);
  // Printing initial setup menu.
  std::cout << "This is a simulation for MTRN2500 Cafe." << std::endl;
  std::cout << "Press [I] to reprint the commands." << std::endl;
  std::cout << "Press [R] to remote control a Supervisor." << std::endl;
  std::cout << "Press [A] to enter the auto mode." << std::endl;
  std::cout << "Press [Q] to quit all controllers." << std::endl;
  // Cooldown counter so it doesn't print the command out multiple times.
  auto i = 0;
  while (step(mTimeStep) != -1) {
    auto key = mKeyboard->getKey();
    if (key != -1 && i > 10) {
    //setting cooldown to 0
    i = 0;
      switch(key) {
        // Case for pressing i or I.
        case 73 : case 65609 :
          std::cout << "This is a simulation for MTRN2500 Cafe.\n";
          std::cout << "Press [I] to reprint the commands.\n";
          std::cout << "Press [R] to remote control a Supervisor.\n";
          std::cout << "Press [A] to enter the auto mode.\n";
          std::cout << "Press [Q] to quit all controllers." << std::endl;
          break;
        // Case for pressing r or R.
        case 82 : case 65618 :
          std::cout << "Entering remote control mode" << std::endl;
          remoteControl();
          return;
        // Case for pressing a or A.
        case 65 : case 65601 :
          std::cout << "Entering auto mode." << std::endl;
          //Auto;
          break;
        // Case for pressing q or Q.
        case 81 : case 65617 :
          std::cout << "Quiting" << std::endl;
          return;
        default :
          std::cout << "Command not found." << std::endl;
          break;
      }
    }
    i++;
  };
  return;
}

void Person::RemoteMode(int timeStep) {
  double velocity = 0.5*MAX_MOTOR_SPEED;
  int cooldown = 5;
  std::cout << "entering remote" << std::endl;
  
  auto leftMotor = static_cast<webots::Motor*>(getMotor("left wheel motor"));
  auto rightMotor = static_cast<webots::Motor*>(getMotor("right wheel motor"));

  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  leftMotor->setVelocity(0.0);
  rightMotor->setVelocity(0.0);


  auto keyboard = getKeyboard();
  keyboard->enable(timeStep);
  
  while (step(timeStep) != -1) {
    cooldown++;
    auto key = keyboard->getKey();
    if(key != -1) {
              // Move forward.
        if(key == mForward) {
          leftMotor->setVelocity(velocity);
          rightMotor->setVelocity(velocity);
        } else if (key == mRight) {
        // Turn right.
          leftMotor->setVelocity(velocity);
          rightMotor->setVelocity(-1*velocity);
        } else if (key == mLeft) {
        // Turn left.
          leftMotor->setVelocity(-1*velocity);
          rightMotor->setVelocity(velocity);
        }
      switch(key) {

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
          exit();
          return;
        default :
          break;
        
      }
    } 

  };
}