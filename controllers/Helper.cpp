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
#include <sstream>
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
    auto sendMessageAll(std::string message);
    auto sendMessage(std::string message, int channel);
    std::string getMessage();
    virtual void rotate90(std::string direction);
    void move(double distance);
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
    double mOrderPositionX;
    double mOrderPositionZ;
    double mPickUpPositionX;
    double mPickUpPositionZ;
    double mStepRatio;
};

void Person::rotate90(std::string direction) {
  return;
}


auto Person::sendMessage(std::string message, int channel) {

  mEmitter->setChannel(channel);
  if (mEmitter->send(message.c_str(), message.size() + 1)) {
    // std::cout << "Message sent" << std::endl;
    return;
  }
  std::cout << "Failed to send a message :(" << std::endl;

  
  return;
}

auto Person::sendMessageAll(std::string message) {
  int i = 0;
  while(i < 6) {
    mEmitter->setChannel(i);
    if (mEmitter->send(message.c_str(), message.size() + 1)) {
      // std::cout << "Message sent" << std::endl;
        
    }else {
      std::cout << "Failed to send a message :(" << std::endl;
    }
    i++;
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
      // std::cout << "I have recevied: " << message << std::endl;
      return message;
    }
    return message;
  }

  
}
void Person::autoMode() {
  return;
}

class Staff : public Person {
  public:
  Staff(int forward, int left, int right, int channel) : Person(forward, left, right, channel) {
    mOrderPositionX = 0.63; 
    mOrderPositionZ = 0.375;
    mPickUpPositionX = 0.63;
    mPickUpPositionZ = -0.375;
    mStepRatio = 0.0038;
  }
  void autoMode();
  void rotate90(std::string direction);

};

class Customer : public Person {
  public:
  Customer(int forward, int left, int right, int channel) : Person(forward, left, right, channel) {
    mOrderPositionX = 0.45; 
    mOrderPositionZ = 0.375;
    mPickUpPositionX = 0.45;
    mPickUpPositionZ = -0.375;
    mStepRatio = 0.00265;
  }
  void rotate90(std::string direction);
  void autoMode();
  private:
  double money;
};

void Customer::rotate90(std::string direction) {
  double coef;
  if (direction == "C") {
    coef = 0.5;
  } else {
    coef = -0.5;
  }
  int i = 0;
  mLeftMotor->setPosition(INFINITY);
  mRightMotor->setPosition(INFINITY);
  mLeftMotor->setVelocity(coef*MAX_MOTOR_SPEED);
  mRightMotor->setVelocity(-coef*MAX_MOTOR_SPEED);

  
  while(step(mTimeStep) != -1) {
    if (i > 29) {
      break;
    }
    i++;
  }
  mLeftMotor->setVelocity(0);
  mRightMotor->setVelocity(0);
}

void Staff::rotate90(std::string direction) {
  double coef;
  if (direction == "C") {
    coef = 0.5;
  } else {
    coef = -0.5;
  }
  int i = 0;
  mLeftMotor->setPosition(INFINITY);
  mRightMotor->setPosition(INFINITY);
  mLeftMotor->setVelocity(coef*MAX_MOTOR_SPEED);
  mRightMotor->setVelocity(-coef*MAX_MOTOR_SPEED);

  
  while(step(mTimeStep) != -1) {
    if (i > 20) {
      break;
    }
    i++;
  }
  mLeftMotor->setVelocity(0);
  mRightMotor->setVelocity(0);
}


void Customer::autoMode() {
  auto order = getMessage();
  rotate90("C");
  mLeftMotor->setPosition(INFINITY);
  mRightMotor->setPosition(INFINITY);
  mLeftMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
  mRightMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
  int i = 0;

  double translationX = std::stod(getMessage());
  double translationZ = std::stod(getMessage());
  double distanceX = (mOrderPositionX - translationX);
  move(distanceX); 
  
  double distanceZ = (mOrderPositionZ - translationZ);
  if (distanceZ < 0) {
    rotate90("CC");
    move(-distanceZ);
    rotate90("C");
  } else {
    rotate90("C");
    rotate90("CC");
  }
  sendMessage(order, 5); 
  return;
}

void Person::move(double distance) {
  int stop = distance/mStepRatio;
  std::cout << stop << std::endl;
  int i = 0;
  mLeftMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
  mRightMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
  while (step(mTimeStep) != -1) {
    if(i > stop) {
      break;
    }
    i++;
  };
  mLeftMotor->setVelocity(0);
  mRightMotor->setVelocity(0);  
  return;
}

void Staff::autoMode() {
  std::cout << "staff auto" << std::endl;
  double translationX = std::stod(getMessage());
  double translationZ = std::stod(getMessage());
  while(step(mTimeStep) != -1) {
    std::string message = getMessage();
    if(message != "") {
      std::cout << message << std::endl; 
      break;
    }
  }
  rotate90("CC");
  auto message = getMessage();

  double distanceX = (mOrderPositionX - translationX);
  move(-distanceX);
  rotate90("C");
  double distanceZ = (mOrderPositionZ - translationZ);
  move(-distanceZ + 0.23);
  rotate90("CC");
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
  sendMessageAll("exit");
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
  auto sendMessageAll(std::string message);
  auto sendMessage(std::string message, int channel);
  std::string getMessage();
  void sendPosition(int channel);
  private:
  int mChannel;
  int mTimeStep;
  webots::Keyboard *mKeyboard;
  webots::Emitter *mEmitter;
  webots::Receiver *mReceiver;
};

auto Director::sendMessage(std::string message, int channel) {
  
  mEmitter->setChannel(channel);
  if (mEmitter->send(message.c_str(), message.size() + 1)) {
    return;
  }
  std::cout << "Failed to send a message :(" << std::endl;
  
  
  return;
}

auto Director::sendMessageAll(std::string message) {
  int i = 0;
  while(i < 6) {
    mEmitter->setChannel(i);
    if (mEmitter->send(message.c_str(), message.size() + 1)) {
      // std::cout << "Message sent" << std::endl;
        
    }
  std::cout << "Failed to send a message :(" << std::endl;
    i++;
  }
  return;
}

std::string Director::getMessage() {
  mReceiver->setChannel(mChannel);
  mReceiver->enable(mTimeStep);
  std::string message;
  while (step(mTimeStep) != -1) {
    if (mReceiver->getQueueLength() > 0) {
      message =
          static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
      mReceiver->nextPacket();  // Pops queue.
      // std::cout << "I have recevied: " << message << std::endl;
      return message;
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
  
  
  std::vector<std::string> orders {"1", "latte"};
  for (auto i = 0; i < orders.size() -1; i += 2) {
    if (orders.at(i) == "1") {
      sendMessage("A", 1);
      sendMessage("A", 5);
      sendMessage(orders.at(i+1), 1);
      sendPosition(1);

    } else if (orders.at(i) == "2") {
      sendMessage("A", 2);
      sendMessage(orders.at(i+1), 2);
    } else if (orders.at(i) == "3") {
      sendMessage("A", 3);
      sendMessage(orders.at(i+1), 3);
    } else if (orders.at(i) == "4") {
      sendMessage("A", 4);
      sendMessage(orders.at(i+1), 4);
    }
  }
  return;
}

void Director::sendPosition(int channel) {
  std::string robot;
  switch (channel) {
    case 1:
      robot = "CUSTOMER1";
      break;
    case 2:
      robot = "CUSTOMER2";
      break;
    case 3:
      robot = "CUSTOMER3";
      break;
    case 4:
      robot = "CUSTOMER4";
      break;
    default:
      std::cout << "Robot not found." << std::endl;
}
  webots::Node *customer{getFromDef(robot)};

  if (!customer) {
   throw std::runtime_error("Failed to get CUSTOMER1!");
  }

  webots::Node *staff {getFromDef("STAFF")};
  if (!staff) {
     throw std::runtime_error("Failed to get STAFF!");
  }


  
  webots::Field *translationFieldS {staff->getField("translation")};
  const double *translationValuesS {translationFieldS->getSFVec3f()};
  webots::Field *translationFieldC {customer->getField("translation")};
  const double *translationValuesC {translationFieldC->getSFVec3f()};

    
  std::string tempX = std::to_string(translationValuesC[0]);
  std::string tempZ = std::to_string(translationValuesC[2]);
  std::string tempY = std::to_string(translationValuesC[1]);

  sendMessage(tempX, channel);
  sendMessage(tempZ, channel);
  tempX = std::to_string(translationValuesS[0]);
  tempZ = std::to_string(translationValuesS[2]);
  sendMessage(tempX, 5);
  sendMessage(tempZ, 5);

  idle();
    
    
  
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
  auto message {"exit"};
  sendMessageAll(message);
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
  sendMessage(mode, channel);
  idle();
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
          mLeftMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
          mRightMotor->setVelocity(-0.5*MAX_MOTOR_SPEED);
        } else if (key == mLeft) {
        // Turn left.
          mLeftMotor->setVelocity(-0.5*MAX_MOTOR_SPEED);
          mRightMotor->setVelocity(0.5*MAX_MOTOR_SPEED);
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

