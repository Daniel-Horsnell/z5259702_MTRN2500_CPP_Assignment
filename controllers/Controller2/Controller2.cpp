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
// From Tute.
std::ifstream read(std::string filename) {
    std::ifstream in{};
    
    in.open(filename, std::ios::in);
    if (!in) {
        std::cout << "Can not open file." << std::endl;
    }
    return in;
}
int main(int argc, char **argv) {
    // Getting starting money.
    auto input {read("../Starting.csv")};
    std::string line;

    double money {0.0};
    getline(input, line);
    while(getline(input,line)) {
      if(line[0] == '2') {
        int size = line.length();
        std::string temp = line.substr(2,size);
        money = std::stod(temp);
      }
    }

    auto customer2 = Customer(87, 65, 68, 2, money, -1.395, 0.34);

    int timeStep = (int)customer2.getBasicTimeStep();
    bool wasAuto = false;

    while (customer2.step(timeStep) != -1) {
        // Constantly recieving messages until told to do otherwise.

        std::string message = customer2.getMessage();
        if(message == "2R") {
            customer2.RemoteMode();
            break;
        } else if (message == "exit") {
            break;
        } else if (message == "A") {
            customer2.autoMode();
            wasAuto = true;
        }

    };
    if (wasAuto) {
      customer2.showAccount();
    }


    return 0;
}
