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
	std::string NormalizeString(const std::string& str);
	
	std::string ToLower(const std::string & str);
	
	std::string UTF8_to_CP1251(std::string const& utf8);

	void RemoveIndirectConnections(std::vector<std::shared_ptr<UserGraph>>& users);

	double CosineSimilarity(const std::unordered_map<std::string, int>& vec1, const std::unordered_map<std::string, int>& vec2);

	std::unordered_map<std::string, int> SplitIntoWords(const std::string& str);

	void CheckAndAddCosineSimilarity(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
		const std::string& userField, const std::string& otherUserField, int weightMultiplier);

	void CheckAndAddSimilaritySchool(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
		const std::vector<std::shared_ptr<School>>& userSchools, const std::vector<std::shared_ptr<School>>& otherUserSchools, int weight);

	void CheckAndAddSimilarityFaculty(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
		const std::string& userField, const std::string& otherUserField, int weight, bool removeNewlines);

	void CheckAndAddSimilarity(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
		const std::string& userField, const std::string& otherUserField, int weight);

	void AddSimilarities(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode);

	std::string GetSimilarParts(const std::unordered_set<std::string>& similarParts);
};

#endif // FACTORY_H

