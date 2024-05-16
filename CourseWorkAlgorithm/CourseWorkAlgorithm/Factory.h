#pragma once
#ifndef FACTORY_H
#define FACTORY_H

#include "User.h"
#include <memory>
#include <vector>
#include <iostream>
#include <map>

struct UserGraph
{
	UserData user;
	std::map<std::shared_ptr<UserGraph>, int> connections;
	//std::map<std::shared_ptr<UserGraph>, std::string>   
};

class Graph
{
public:
	void AddConnections(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output);
	void SaveData(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output);

};

#endif // FACTORY_H

