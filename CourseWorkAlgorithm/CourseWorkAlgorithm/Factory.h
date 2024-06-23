#pragma once
#ifndef FACTORY_H
#define FACTORY_H

#include "User.h"
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <list>

struct UserGraph
{
	UserData user;
	std::map<std::shared_ptr<UserGraph>, std::pair<int, std::unordered_set<std::string>>> connections;
};

class Graph
{
public:
	void AddConnections(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output);
	
	void SaveData(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output);

private:
	
	bool HasSimilarWords(const std::string & str1, const std::string& str2, std::string& similar);

	std::string NormalizeString(const std::string& str);
	
	std::string ToLower(const std::string & str);
	
	std::string UTF8_to_CP1251(std::string const& utf8);

	void RemoveIndirectConnections(std::vector<std::shared_ptr<UserGraph>>& users);

	void AddSimilarities(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode);

	double CosineSimilarity(const std::unordered_map<std::string, int>& vec1, const std::unordered_map<std::string, int>& vec2);

	std::unordered_map<std::string, int> SplitIntoWords(const std::string& str);

};

#endif // FACTORY_H

