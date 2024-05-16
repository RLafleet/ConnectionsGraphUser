#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "User.h"
#include "Factory.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <nlohmann/json.hpp>

class Controller {
public:
    Controller();
    Controller(const std::string& filename);

    void ReadUserDataFromFile(const std::string& filename);
    void AddUser(const std::shared_ptr<UserData>& userData);
    std::string UTF8_to_CP1251(std::string const& utf8);
    void RemoveUser(int userId);
    void PrintUsers();

private:
    std::vector<std::shared_ptr<UserGraph>> users;
};

#endif // CONTROLLER_H
