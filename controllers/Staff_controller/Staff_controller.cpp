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
#include <string>
#include <vector>
#include <fstream>


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
    auto in {read("../Starting.csv")};
    std::string entry;
    double money {0.0};
    getline(in, entry);
    while(getline(in,entry)) {
        if(entry[0] == '5') {
            int size = entry.length();
            std::string temp = entry.substr(2,size);
            money = std::stod(temp);
        }
    }
    // Getting menu.
    auto input {read("../Menu.csv")};
    std::vector <std::string> menu;
    std::string line;
    getline(input, line);
    while(std::getline(input, line)) {
        menu.push_back(line);
    }

    auto staff = Staff(315, 314, 316, 5, menu, money);

    int timeStep = (int)staff.getBasicTimeStep();



    while (staff.step(timeStep) != -1) {
        // Constantly recieving messages until told to do something.
        std::string message = staff.getMessage();

        if(message == "5R") {
            staff.RemoteMode();
            break;
        } else if (message == "exit") {
            return 0;
        } else if (message == "A") {
            staff.autoMode();
            staff.showAccount(money);
        }
        

    };




    return 0;
}



