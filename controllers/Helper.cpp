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
#include <iterator>
#include <iomanip>



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
        mKeyboard->enable(mTimeStep);
        mEmitter = getEmitter("emitter");
        mReceiver = getReceiver("receiver");
        mReceiver->enable(mTimeStep);
        mLeftMotor = static_cast<webots::Motor*>(getMotor("left wheel motor"));
        mRightMotor = static_cast<webots::Motor*>(getMotor("right wheel motor"));

    }
    auto exit();
    virtual void showAccount();
    virtual void autoMode();
    void RemoteMode();
    auto sendMessageAll(std::string message);
    auto sendMessage(std::string message, int channel);
    std::string getMessage();
    virtual void rotate90(std::string direction);
    void move(double distance);
    void navigate(double X, double Z, double rotation);
    virtual bool inMenu(std::string order, double & price, double & time);
    void updatePosition();

    protected:
    double const cMaxSpeed {6.28};
    int mForward;
    int mRight;
    int mLeft;
    int mTimeStep;
    int mChannel;
    // Parts of the robot.
    webots::Keyboard *mKeyboard;
    webots::Emitter *mEmitter;
    webots::Receiver *mReceiver;
    webots::Motor *mLeftMotor;
    webots::Motor *mRightMotor;
    // Order position.
    double mOrderPositionX;
    double mOrderPositionZ;
    // Pick up position.
    double mPickUpPositionX;
    double mPickUpPositionZ;
    // A constant used to find the distance travelled in a timestep.
    double mStepRatio;
    // Start locations.
    double mStartX;
    double mStartZ;
    // Current locations.
    double mCurrX;
    double mCurrZ;
    double mCurrRotation;
    double mMoney;
};

void Person::updatePosition() {
    mCurrX = std::stod(getMessage());
    mCurrZ = std::stod(getMessage());
    mCurrRotation = std::stod(getMessage());
    return;
}

void Person::showAccount() {
    return;
}

bool Person::inMenu(std::string order, double & price, double & time) {
    return true;
}

void Person::rotate90(std::string direction) {
    return;
}

auto Person::sendMessage(std::string message, int channel) {

    mEmitter->setChannel(channel);
    mEmitter->send(message.c_str(), message.size() + 1);

  return;
}

auto Person::sendMessageAll(std::string message) {
    int i = 0;
    // Looping through all channels to send the same message to all.
    while(i < 6) {
        mEmitter->setChannel(i);
        mEmitter->send(message.c_str(), message.size() + 1);

        i++;
    }
    return;
}

std::string Person::getMessage() {
    mReceiver->setChannel(mChannel);
    std::string message {""};
    while (step(mTimeStep) != -1) {
        if (mReceiver->getQueueLength() > 0) {
            message =
                static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
            mReceiver->nextPacket();  // Pops queue.

            // Retruns the message if it is not empty. This is so the function 
            // pauses the robot until a message is recieved.
            if (message != "") {
                return message;
            }
        }
    
    };
  return message;
  
}

void Person::autoMode() {
  return;
}

class Staff : public Person {
    public:
    Staff(int forward, int left, int right, int channel, std::vector <std::string> & menu, double money) : Person(forward, left, right, channel) {
      mOrderPositionX = 0.8; 
      mOrderPositionZ = 0.375;
      mPickUpPositionX = 0.8;
      mPickUpPositionZ = -0.375;
      mStepRatio = 0.0026;
      mMenu = menu;
      mStartX = 1.3;
      mStartZ = 0.875;
      mMoney = money;
    }
    void autoMode();
    void rotate90(std::string direction);
    bool inMenu(std::string order, double & price, double & time);
    void showAccount(double money);

    private:
    std::vector<std::string> mMenu;
    std::vector<std::string> mAccount;

};

class Customer : public Person {
    public:
    Customer(int forward, int left, int right, int channel, double money, double startX, double startZ) : Person(forward, left, right, channel) {
      mOrderPositionX = 0.45; 
      mOrderPositionZ = 0.375;
      mPickUpPositionX = 0.45;
      mPickUpPositionZ = -0.375;
      mStepRatio = 0.00258;
      mStartX = startX;
      mStartZ = startZ;
      mMoney = money;
    }
    void rotate90(std::string direction);
    void autoMode();
    void showAccount();
    private:

};

void Customer::showAccount() {
    std::cout << "Customer " << mChannel << ": My current balance is " << mMoney << " dollars" <<std::endl;
    return;
}

bool Staff::inMenu(std::string order, double & price, double & time) {
    std::string time_str;
    std::string price_str;
    std::string item;
    
    for (std::vector<std::string>::const_iterator iter = mMenu.cbegin(); iter!=mMenu.cend(); ++iter) {
      
        std::stringstream menuItem (*iter);
        // Separating the items in the menu.
        std::getline(menuItem, item, ',');
        std::getline(menuItem, time_str, ',');
        std::getline(menuItem, price_str, ',');
        price = std::stod(price_str);
        time = std::stod(time_str);
        // If the item is in the menu it return true. Since non-const references
        // were used the price and time are changed to the correct values.
        if(item == order) {
            return true;
        }
      
    }
    return false;
}

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
    mLeftMotor->setVelocity(coef*cMaxSpeed);
    mRightMotor->setVelocity(-coef*cMaxSpeed);

    // Loop which waits a given amount of time for the robot to turn 90 degrees.
    while(step(mTimeStep) != -1 && i < 30) {
        i++;
    }
    //Stopping the robot.
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
    mLeftMotor->setVelocity(coef*cMaxSpeed);
    mRightMotor->setVelocity(-coef*cMaxSpeed);

    // A timer which keeps the robot turning till 90 degrees.
    while(step(mTimeStep) != -1 && i < 20) {
      i++;
    }
    // Stopping the robot from turning.
    mLeftMotor->setVelocity(0);
    mRightMotor->setVelocity(0);
}

void Customer::autoMode() {
    // Recieving order instructions from director.
    auto order {getMessage()};
    mLeftMotor->setPosition(INFINITY);
    mRightMotor->setPosition(INFINITY);

    updatePosition();
    double distanceX = (mOrderPositionX - mCurrX);
    double distanceZ = (mOrderPositionZ - mCurrZ);

    std::cout << "Customer " << mChannel << ": I am heading to order counter" << std::endl;
    navigate(distanceX, distanceZ, mCurrRotation);
    std::cout << "Customer " << mChannel << ": Hi Staff, I want to order "<< order << std::endl;
    // Sending order to staff.
    sendMessage(order, 5); 

    
    // Enquiring Director for position.
    sendMessage(std::to_string(mChannel), 0); 
    updatePosition();
    // Getting either the price or not in menu from staff.
    std::string message = getMessage();
    if (message == "Not in menu") {
        std::cout << "Customer " << mChannel << ": I am returning to starting point" << std::endl;
        navigate(mCurrX -mStartX, mCurrZ- mStartZ, mCurrRotation);
        rotate90("C");
        sendMessage("exit", 0);
        return;
    } 
    
    // Converting price to double.
    double price = std::stod(message);
    if (price > mMoney) {
        sendMessage("Oops", 5);
        std::cout << "Customer " << mChannel << ": Hi Staff, oops, I can't afford it" << std::endl;
        std::cout << "Customer " << mChannel << ": I am returning to starting point" << std::endl;
        navigate(mCurrX -mStartX, mCurrZ- mStartZ, mCurrRotation);
        rotate90("C");
        sendMessage("exit", 0);
        return;
    } 
    // Sending message to staff saying that the customer will buy the product.
    sendMessage("Y", 5);
    std::cout << "Customer " << mChannel << ": Hi Staff, I will buy it" << std::endl;
    navigate(mCurrX -mStartX, mCurrZ- mStartZ, mCurrRotation);
    rotate90("C");
    
    mMoney -= price;
    // Returns ready but is not useful to know that information. The main purpose
    // Of this function is to pause the customer until staff is ready.
    getMessage(); 
    std::cout << "Customer " << mChannel << ": I am heading to pickup counter" << std::endl;
    // Enquiring director for position.
    sendMessage(std::to_string(mChannel), 0);
    updatePosition();
    navigate(mPickUpPositionX - mCurrX, mPickUpPositionZ - mCurrZ, mCurrRotation);
    // Telling staff that the order has been picked up.

    sendMessage("Thanks", 5);
    std::cout << "Customer " << mChannel << ": I got my " << order << std::endl;
    std::cout << "Customer " << mChannel << ": I am returning to starting point" << std::endl;
    // Enquiring about position.
    
    sendMessage(std::to_string(mChannel), 0);
    updatePosition();
    navigate(mCurrX -mStartX, mCurrZ- mStartZ, mCurrRotation);
    rotate90("C");
    sendMessage("exit", 0);
    return;
}

void Person::navigate(double distanceX, double distanceZ, double rotation) {

    if(distanceX < 0) {
        if(rotation < 0.1 && rotation > -0.1) {
            rotate90("CC");
            move(-distanceX/2);
        } else if (rotation < 1.7 && rotation > 1.5) {
            rotate90("C");
            rotate90("C");
            move(-distanceX/2);
        } else if (rotation > 3 && rotation < 3.3) {
            rotate90("C");
            move(-distanceX/2);
        } else {
            move(-distanceX/2);
        }

        if (distanceZ < 0) {
            rotate90("C");
            move(-distanceZ);
            rotate90("CC");
            move(-distanceX/2);
        } else {
            rotate90("CC");
            move(distanceZ);
            rotate90("C");
            move(-distanceX/2);
        }
    } else {
        if(rotation < 0.1 && rotation > -0.1) {
            rotate90("C");
            move(distanceX/2);
        } else if (rotation < 1.7 && rotation > 1.5) {
            move(distanceX/2);
        } else if (rotation > 3 && rotation < 3.3) {
          
            rotate90("C");
            move(distanceX/2);
        } else {
          
            rotate90("C");
            rotate90("C");
            move(distanceX/2);
        }

        if (distanceZ < 0) {
            rotate90("CC");
            move(-distanceZ);
            rotate90("C");
            move(distanceX/2);
        } else {
            rotate90("C");
            move(distanceZ);
            rotate90("CC");
            move(distanceX/2);
        }
    }
  
  
}

void Person::move(double distance) {
    // Calculating the amount of timesteps to travel a certain distance.
    int stop = distance/mStepRatio;
    
    int i {0};
    mLeftMotor->setVelocity(0.5*cMaxSpeed);
    mRightMotor->setVelocity(0.5*cMaxSpeed);
    // moving for stop amount of timesteps.
    while (step(mTimeStep) != -1 && i < stop) {
        i++;
    };
    // Stopping movement.
    mLeftMotor->setVelocity(0);
    mRightMotor->setVelocity(0);  
    return;
}

void Staff::showAccount(double money) {
  std::string filename {"../Account.csv"};
  // Write to file
  std::ofstream ofst {filename}; 

  if (!ofst) {
  std::cerr << "error: open file for output failed!\n";
  return;
  }
  // Setting out the initial layout.
  ofst << std::setw(20) << "Time(s)" << ","
    << std::setw(20) << "Item" << ","
    << std::setw(20) << "Customer" << ","
    << std::setw(20) << "Account Balance($)" << '\n'; 
  ofst << std::setw(20) << "0" << ","
    << std::setw(20) << " "  << ","
    << std::setw(20) << " "  << ","
    << std::setw(20) << std::to_string(money) << '\n';
  int i = 1;
  for (std::vector<std::string>::const_iterator iter = mAccount.cbegin(); iter!=mAccount.cend(); ++iter) {
        // Entering values from accout vector.
        ofst << std::setw(20) << *iter << ",";
        if (i % 4 == 0) {
          // Entering a new line every 4 values.
          ofst << '\n';
        }
        i++;
    }
  ofst.close();
  return;
}

void Staff::autoMode() {
    // Recieving customer channel from director.
    std::string channel_str = getMessage();
    int channel = std::stoi(channel_str);
    updatePosition();
    // Recieving order from customer.
    std::string message = getMessage();
    
    double distanceX = (mOrderPositionX - mCurrX);

    double distanceZ = (mOrderPositionZ - mCurrZ);
    std::cout << "Staff: I am heading to order counter" << std::endl;
    navigate(distanceX, distanceZ, mCurrRotation);

    double price;
    double time;

    if(!inMenu(message, price, time)) {
        std::cout << "Staff: Hi Customer " << channel_str << ", oh no, we don't have " << message <<  " in our menu" << std::endl;
        sendMessage("Not in menu", channel); 
        // Enquiring director for position.
        sendMessage("5", 0);
        updatePosition();
        // Returning to start position.
        navigate(mCurrX - mStartX, mCurrZ - mStartZ, mCurrRotation);
        rotate90("CC");
        return;
    }
    std::cout << "Staff: Hi Customer " << channel_str << ", the price for " << message <<  " is " << price <<" dollars" << std::endl;
    auto price_str = std::to_string(price);
    // Sending price to customer.
    sendMessage(price_str, channel);
    // Getting answer from customer (on whether they are buying the item).
    auto answer = getMessage();

    if(answer == "Oops") {
        // Enquiring director for position.
        sendMessage("5", 0);
        updatePosition();
        navigate(mCurrX - mStartX, mCurrZ - mStartZ, mCurrRotation);
        rotate90("CC");
        return;
    }
    mMoney += price;
    // Adding order to account.
    mAccount.push_back(std::to_string(getTime()));
    mAccount.push_back(message);
    mAccount.push_back(channel_str);
    mAccount.push_back(std::to_string(mMoney));
    //figuring out the amount of timesteps that the staff needs to wait.
    int total_timeSteps = 1000.0/32.0*time;

    int counter = 0;
    // Timer loop for waiting.
    while(step(mTimeStep) != -1 && counter < total_timeSteps){
      counter++;
    }

    std::cout << "Staff: Hi Customer " << channel_str << ", your " << message << " is ready, please proceed to pickup counter" << std::endl;
    // Telling customer the order is ready.
    sendMessage("Ready", channel);
    // Enquiring director for position.
    sendMessage("5", 0);
    updatePosition();
    std::cout << "Staff: I am heading to order counter" << std::endl;
    navigate(mCurrX - mPickUpPositionX, mCurrZ - mPickUpPositionZ, mCurrRotation);
    getMessage(); // Will be Thanks.
    // Enquiring director for position.
    sendMessage("5", 0);
    updatePosition();
    navigate(mCurrX - mStartX, mCurrZ - mStartZ, mCurrRotation);

    rotate90("CC");
  
    return;
}

auto Person::exit() {
  sendMessageAll("exit");
  return;
}

class Director : public webots::Supervisor {
    public:
    Director(std::vector <std::string> & orders) : Supervisor() {
        mTimeStep = getBasicTimeStep();
        mKeyboard = getKeyboard();
        mKeyboard->enable(mTimeStep);
        mEmitter = getEmitter("emitter");
        mReceiver = getReceiver("receiver");
        mReceiver->enable(mTimeStep);
        mChannel = 0;
        mOrders = orders;
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
    std::vector <std::string> mOrders;
};

auto Director::sendMessage(std::string message, int channel) {
  
    mEmitter->setChannel(channel);
    mEmitter->send(message.c_str(), message.size() + 1);

    return;
}

auto Director::sendMessageAll(std::string message) {
  int i = 0;
  // Looping through all channels.
  while(i < 6) {
    mEmitter->setChannel(i);
    mEmitter->send(message.c_str(), message.size() + 1);

    i++;
  }
  return;
}

auto Director::exit() {
  auto message {"exit"};
  sendMessageAll(message);
  return;
}

std::string Director::getMessage() {
    mReceiver->setChannel(mChannel);
    std::string message = "";
    while (step(mTimeStep) != -1) {
        if (mReceiver->getQueueLength() > 0) {
          message =
              static_cast<std::string>((static_cast<const char*>(mReceiver->getData())));
          mReceiver->nextPacket();  // Pops queue.
          // This function keeps looping through until a message is read in.
          if(message != "") {
              return message;
          }
      }
    }
    return message;
}

auto Director::autoMode() {
  
    double time = getTime();
    for (long unsigned int i = 0; i < mOrders.size() -1; i += 2) {
        if (mOrders.at(i) == "1") {
            // Telling staff and customer to enter auto mode and sending their start positions.
            sendMessage("A", 1);
            sendMessage("A", 5);
            sendMessage("1", 5);
            sendMessage(mOrders.at(i+1), 1);
            sendPosition(1);
            sendPosition(5);
            idle();

        } else if (mOrders.at(i) == "2") {
            // Telling staff and customer to enter auto mode and sending their start positions.
            sendMessage("A", 2);
            sendMessage("A", 5);
            sendMessage("2", 5);
            sendMessage(mOrders.at(i+1), 2);
            sendPosition(2);
            sendPosition(5);
            idle();
        } else if (mOrders.at(i) == "3") {
            // Telling staff and customer to enter auto mode and sending their start positions.
            sendMessage("A", 3);
            sendMessage("A", 5);
            sendMessage("3", 5);
            sendMessage(mOrders.at(i+1), 3);
            sendPosition(3);
            sendPosition(5);
            idle();
        } else if (mOrders.at(i) == "4") {
            // Telling staff and customer to enter auto mode and sending their start positions.
            sendMessage("A", 4);
            sendMessage("A", 5);
            sendMessage("4", 5);
            sendMessage(mOrders.at(i+1), 4);
            sendPosition(4);
            sendPosition(5);
            idle();
        }
    }
    sendMessageAll("EA");

    double totalTime = getTime() - time;
    std::cout << "Director: The whole auto mode takes up " << totalTime << "s" << std::endl;
    return;
}

void Director::sendPosition(int channel) {
    std::string robot;
    std::string str_channel;
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
      case 5:
        robot = "STAFF";
        break;
      default:
        std::cout << "Robot not found." << std::endl;
  }
    webots::Node *customer{getFromDef(robot)};

    if (!customer) {
    throw std::runtime_error("Failed to get CUSTOMER!");
    }

    webots::Field *translationFieldC {customer->getField("translation")};
    const double *translationValuesC {translationFieldC->getSFVec3f()};
    webots::Field *mCurrRotationieldC {customer->getField("rotation")};
    const double *rotationValuesC {mCurrRotationieldC->getSFRotation()};
    // Storing X values.
    std::string tempX = std::to_string(translationValuesC[0]);
    // Storing Z values.
    std::string tempZ = std::to_string(translationValuesC[2]);
    // Storing the rotation angle. (by multiplying by the y dimension we always get the correct angle.)
    std::string tempR = std::to_string(rotationValuesC[1]*rotationValuesC[3]);

    // sending X Z and rotation values.
    sendMessage(tempX, channel);
    sendMessage(tempZ, channel);
    sendMessage(tempR, channel);

    return;
}

void Director::idle() {
  while(step(mTimeStep) != -1) {
    std::string message = getMessage();
    if (message == "exit") {
      return;
    } else if (message == "1") {
      sendPosition(1);
    } else if (message == "2") {
      sendPosition(2);
    } else if (message == "3") {
      sendPosition(3);
    } else if (message == "4") {
      sendPosition(4);
    } else if (message == "5") {
      sendPosition(5);
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
    // Printing initial setup menu.
    std::cout << "Director: This is a simulation for MTRN2500 Cafe." << std::endl;
    std::cout << "Director: Press [I] to reprint the commands." << std::endl;
    std::cout << "Director: Press [R] to remote control a Supervisor." << std::endl;
    std::cout << "Director: Press [A] to enter the auto mode." << std::endl;
    std::cout << "Director: Press [Q] to quit all controllers." << std::endl;
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
              std::cout << "Director: This is a simulation for MTRN2500 Cafe.\n";
              std::cout << "Director: Press [I] to reprint the commands.\n";
              std::cout << "Director: Press [R] to remote control a Supervisor.\n";
              std::cout << "Director: Press [A] to enter the auto mode.\n";
              std::cout << "Director: Press [Q] to quit all controllers." << std::endl;
              break;
            // Case for pressing r or R.
            case 82 : case 65618 :
              std::cout << "Director: Entering remote control mode" << std::endl;
              remoteControl();
              return;
            // Case for pressing a or A.
            case 65 : case 65601 :
              std::cout << "Director: Auto mode start" << std::endl;
              autoMode();
              exit();
              return;
              
            // Case for pressing q or Q.
            case 81 : case 65617 :
              std::cout << "Director: Quiting" << std::endl;
              exit();
              return;
            default :
              std::cout << "Director: Command not found." << std::endl;
              break;
          }
        }
        i++;
    };
    return;
}

void Person::RemoteMode() {
    double velocity = 0.5*cMaxSpeed;
    int cooldown = 5;
    std::cout << "Director: Remote mode start" << std::endl;

    mLeftMotor->setPosition(INFINITY);
    mRightMotor->setPosition(INFINITY);
    mLeftMotor->setVelocity(0.0);
    mRightMotor->setVelocity(0.0);

    while (step(mTimeStep) != -1) {
        cooldown++;
        auto key = mKeyboard->getKey();
        if(key != -1) {
                  // Move forward.
            if(key == mForward) {
              mLeftMotor->setVelocity(velocity);
              mRightMotor->setVelocity(velocity);
            } else if (key == mRight) {
            // Turn right.
              mLeftMotor->setVelocity(0.5*cMaxSpeed);
              mRightMotor->setVelocity(-0.5*cMaxSpeed);
            } else if (key == mLeft) {
            // Turn left.
              mLeftMotor->setVelocity(-0.5*cMaxSpeed);
              mRightMotor->setVelocity(0.5*cMaxSpeed);
            }
          switch(key) {

            // Increase Speed.
            case 61 :
              if(velocity < cMaxSpeed && cooldown >= 5) {
                std::cout << "Increasing speed: ";
                velocity += 0.1*cMaxSpeed;
                std::cout << velocity << std::endl;
                mLeftMotor->setVelocity(velocity);
                mRightMotor->setVelocity(velocity);
                cooldown = 0;
              }
              break;
            // Decrease Speed.
            case 45 :
              if(velocity > 0 && cooldown >= 5) {
                std::cout << "Decreasing speed: ";
                velocity -= 0.1*cMaxSpeed;
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
              mLeftMotor->setVelocity(0);
              mRightMotor->setVelocity(0);
              exit();
              return;
            default :
              break;
            
          }
      } 

    };
}

