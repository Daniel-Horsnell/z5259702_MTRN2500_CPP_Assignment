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
#include <fstream>

#include <vector>

auto constexpr MAX_MOTOR_SPEED = 6.28;

using namespace webots;

class Person : public webots::Robot {
    public:
    Person(int forward, int left, int right, int channel) : Robot() {
        mForward = forward;
        mRight = right;
        mLeft = left;
        mChannel = channel;
        mTimeStep = getBasicTimeStep();
        mKeyboard = getKeyboard();
        mEmitter = getEmitter("emitter");
        mReceiver = getReceiver("receiver");
        mLeftMotor = static_cast<webots::Motor*>(getMotor("left wheel motor"));
        mRightMotor = static_cast<webots::Motor*>(getMotor("right wheel motor"));
    }
    auto exit();
    virtual void autoMode();
    void RemoteMode(int timeStep);
    auto sendMessage(std::string message, int start, int end);
    std::string getMessage();

    protected:
    int mForward;
    int mRight;
    int mLeft;
    int mTimeStep;
    int mChannel;
    webots::Keyboard *mKeyboard;
    webots::Emitter *mEmitter;
    webots::Receiver *mReceiver;
    webots::Motor *mLeftMotor;
    webots::Motor *mRightMotor;

};



auto Person::sendMessage(std::string message, int start, int end) {
  while(start <= end) {
    mEmitter->setChannel(start);
    if (mEmitter->send(message.c_str(), message.size() + 1)) {
        std::cout << "Message sent" << std::endl;
        
    }
    std::cout << "Failed to send a message :(" << std::endl;
    start++;
  }
  return;
}

std::string Person::getMessage() {
  mReceiver->setChannel(mChannel);
  mReceiver->enable(mTimeStep);
  std::string message;
  while (step(mTimeStep) != -1) {
    if (mReceiver->getQueueLength() > 0) {
      message =
          static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
      mReceiver->nextPacket();  // Pops queue.
      std::cout << "I have recevied: " << message << std::endl;
    }
  }

  return message;
}
void Person::autoMode() {
  return;
}

class Staff : public Person {
  public:
  Staff(int forward, int left, int right, int channel) : Person(forward, left, right, channel) {}
  void autoMode();
};

class Customer : public Person {
  public:
  Customer(int forward, int left, int right, int channel) : Person(forward, left, right, channel) {}
  void autoMode();
  private:
  double money;
};

void Customer::autoMode() {
  auto message = getMessage();

  return;
}

void Staff::autoMode() {
  auto message = getMessage();

  return;
}
// auto readCSV(std::string filename) {

//   std::ifstream fileInput;
//   fileInput.open(filename);
//   std::vector<std::string> fileVector;
//   std::string temp

//   while (getline(fileInput, temp)) {
//     fileVector.push_back(temp);
//   }
//   return fileVector;
// }

auto Person::exit() {
  sendMessage("exit", 0, 5);
  return;
}


class Director : public webots::Supervisor {
  public:
  Director() : Supervisor() {
    mTimeStep = getBasicTimeStep();
    mKeyboard = getKeyboard();
    mEmitter = getEmitter("emitter");
    mReceiver = getReceiver("receiver");
    mChannel = 0;
  }
  void menu();
  void remoteControl();
  auto setRobotMode(std::string mode);
  void idle();
  auto exit();
  auto autoMode();
  auto sendMessage(std::string message, int start, int end);
  std::string getMessage();
  private:
  int mChannel;
  int mTimeStep;
  webots::Keyboard *mKeyboard;
  webots::Emitter *mEmitter;
  webots::Receiver *mReceiver;
};

auto Director::sendMessage(std::string message, int start, int end) {
  while(start <= end) {
    mEmitter->setChannel(start);
    if (mEmitter->send(message.c_str(), message.size() + 1)) {
        std::cout << "Message sent" << std::endl;
        
    }
    std::cout << "Failed to send a message :(" << std::endl;
    start++;
  }
  return;
}

std::string Director::getMessage() {
  mReceiver->setChannel(mChannel);
  std::string message;
  while (step(mTimeStep) != -1) {
    if (mReceiver->getQueueLength() > 0) {
      message =
          static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
      mReceiver->nextPacket();  // Pops queue.
      std::cout << "I have recevied: " << message << std::endl;
    }
  }
  return message;
}

auto Director::autoMode() {
  // webots::Node *staff {getFromDef("STAFF")};
  // if (!staff) {
  //   throw std::runtime_error("Failed to get STAFF!");
  // }
  // webots::Node *customer1{getFromDef("CUSTOMER1")};
  // if (!staff) {
  //   throw std::runtime_error("Failed to get CUSTOMER1!");
  // }
  // webots::Node *customer2{getFromDef("CUSTOMER2")};
  // if (!staff) {
  //   throw std::runtime_error("Failed to get CUSTOMER2!");
  // }
  // webots::Node *customer3{getFromDef("CUSTOMER3")};
  // if (!staff) {
  //   throw std::runtime_error("Failed to get CUSTOMER3!");
  // }
  // webots::Node *customer4{getFromDef("CUSTOMER4")};
  // if (!staff) {
  //   throw std::runtime_error("Failed to get CUSTOMER4!");
  // }
  // webots::Field *translationFieldS {staff->getField("translation")};
  // webots::Field *translationFieldC1 {customer1->getField("translation")};
  // webots::Field *translationFieldC2 {customer2->getField("translation")};
  // webots::Field *translationFieldC3 {customer3->getField("translation")};
  // webots::Field *translationFieldC4 {customer4->getField("translation")};
  // const double *translationValuesS {translationFieldS->getSFVec3f()};
  // const double *translationValuesC1 {translationFieldC1->getSFVec3f()};
  // const double *translationValuesC2 {translationFieldC2->getSFVec3f()};
  // const double *translationValuesC3 {translationFieldC3->getSFVec3f()};
  // const double *translationValuesC4 {translationFieldC4->getSFVec3f()};
  std::vector<std::string> orders {"1latte"};
  for (auto iter = orders.begin(); iter != orders.end(); ++iter) {
    if (iter->at(0) == 1) {
      mEmitter->setChannel(1);
      if (mEmitter->send(iter->c_str(), iter->size() + 1)) {
       std::cout << "Automode customer 1 initiated" << std::endl;
        
      } else {
        std::cout << "Failed to send a message :(" << std::endl;
      }
    }
  }
  return;
}


  // auto i = 0;
  // while(i < 5) {
  //   mEmitter->setChannel(i);
  //   std::string message = "A";
  //   if (mEmitter->send(message.c_str(), message.size() + 1)) {
  //       std::cout << "Automode robot" << i << "initiated" << std::endl;
        
  //   }
  //   std::cout << "Failed to send a message :(" << std::endl;
  //   i++;
  // }
  // readCSV("../../starting.csv");



auto Director::exit() {
  sendMessage("exit", 0 , 5);
  return;
}

void Director::idle() {
  while(step(mTimeStep) != -1) {
    std::string message = getMessage();
    if (message == "exit") {
      return;
    }
  };
}
auto Director::setRobotMode(std::string mode) {
  int channel = mode.at(0);
  channel = channel - 48;
  sendMessage(mode, channel, channel);
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
          autoMode();
          break;
        // Case for pressing q or Q.
        case 81 : case 65617 :
          std::cout << "Quiting" << std::endl;
          exit();
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

  mLeftMotor->setPosition(INFINITY);
  mRightMotor->setPosition(INFINITY);
  mLeftMotor->setVelocity(0.0);
  mRightMotor->setVelocity(0.0);


  auto keyboard = getKeyboard();
  keyboard->enable(timeStep);
  
  while (step(timeStep) != -1) {
    cooldown++;
    auto key = keyboard->getKey();
    if(key != -1) {
              // Move forward.
        if(key == mForward) {
          mLeftMotor->setVelocity(velocity);
          mRightMotor->setVelocity(velocity);
        } else if (key == mRight) {
        // Turn right.
          mLeftMotor->setVelocity(velocity);
          mRightMotor->setVelocity(-1*velocity);
        } else if (key == mLeft) {
        // Turn left.
          mLeftMotor->setVelocity(-1*velocity);
          mRightMotor->setVelocity(velocity);
        }
      switch(key) {

        // Increase Speed.
        case 61 :
          if(velocity < MAX_MOTOR_SPEED && cooldown >= 5) {
            std::cout << "increasing speed" << std::endl;
            velocity += 0.1*MAX_MOTOR_SPEED;
            std::cout << velocity << std::endl;
            mLeftMotor->setVelocity(velocity);
            mRightMotor->setVelocity(velocity);
            cooldown = 0;
          }
          break;
        // Decrease Speed.
        case 45 :
          if(velocity > 0 && cooldown >= 5) {
            std::cout << "decreasing speed" << std::endl;
            velocity -= 0.1*MAX_MOTOR_SPEED;
            std::cout << velocity << std::endl;
            mLeftMotor->setVelocity(velocity);
            mRightMotor->setVelocity(velocity);
            cooldown = 0;
          }
          break;
        // Pause.
        case 32 :
          mLeftMotor->setVelocity(0);
          mRightMotor->setVelocity(0);
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