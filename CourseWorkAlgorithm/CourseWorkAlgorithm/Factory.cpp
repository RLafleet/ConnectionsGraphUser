#include "Factory.h"
#include "Controller.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::vector;

string Graph::UTF8_to_CP1251(const string& utf8)
{
	if (utf8.empty())
		return string();

	int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
	if (wchlen > 0 && wchlen != 0xFFFD)
	{
		std::vector<wchar_t> wbuf(wchlen);
		MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), wbuf.data(), wchlen);
		std::vector<char> buf(wchlen);
		WideCharToMultiByte(1251, 0, wbuf.data(), wchlen, buf.data(), wchlen, 0, 0);

		return string(buf.data(), wchlen);
	}
	return string();
}

string Graph::ToLower(const string& str)
{
	string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

string Graph::NormalizeString(const string& str)
{
	string result;
	for (char ch : str)
	{
		result += (ch == '-') ? ' ' : ch;
	}
	return result;
}

unordered_map<string, int> Graph::SplitIntoWords(const string& str)
{
	unordered_map<string, int> wordCount;
	string normalizedStr = NormalizeString(str);
	std::istringstream iss(normalizedStr);
	string word;
	while (iss >> word)
	{
		word = ToLower(word);
		wordCount[word]++;
	}
	return wordCount;
}

double Graph::CosineSimilarity(const unordered_map<string, int>& vec1, const unordered_map<string, int>& vec2)
{
	double dotProduct = 0.0, normA = 0.0, normB = 0.0;

	for (const auto& pair : vec1)
	{
		auto it = vec2.find(pair.first);
		if (it != vec2.end())
			dotProduct += pair.second * it->second;
		normA += pair.second * pair.second;
	}

	for (const auto& pair : vec2)
	{
		normB += pair.second * pair.second;
	}

	if (normA == 0.0 || normB == 0.0)
		return 0.0;

	return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}

void RemoveNewlines(string& str)
{
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}

void Graph::AddConnections(vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
	for (size_t i = 0; i < users.size(); ++i)
	{
		auto& userNode = users[i];
		std::vector<bool> visited(users.size(), false);

		for (size_t j = i + 1; j < users.size(); ++j)
		{
			if (!visited[j])
			{
				auto& otherUserNode = users[j];
				AddSimilarities(userNode, otherUserNode);
				visited[j] = true;
			}
		}
	}

	RemoveIndirectConnections(users);
}

void Graph::AddSimilarities(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode)
{
	const auto& user = userNode->user;
	const auto& otherUser = otherUserNode->user;

	CheckAndAddSimilarityFaculty(userNode, otherUserNode, user.education, otherUser.education, 3, true);
	CheckAndAddSimilaritySchool(userNode, otherUserNode, user.schools, otherUser.schools, 4);

	CheckAndAddCosineSimilarity(userNode, otherUserNode, user.interests, otherUser.interests, 10);
	CheckAndAddCosineSimilarity(userNode, otherUserNode, user.activities, otherUser.activities, 10);
	CheckAndAddCosineSimilarity(userNode, otherUserNode, user.about, otherUser.about, 10);
	CheckAndAddCosineSimilarity(userNode, otherUserNode, user.movies, otherUser.movies, 10);
	CheckAndAddCosineSimilarity(userNode, otherUserNode, user.books, otherUser.books, 10);
}

void Graph::CheckAndAddSimilarity(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
	const string& userField, const string& otherUserField, int weight)
{
	if (!userField.empty() && !otherUserField.empty() && userField == otherUserField)
	{
		userNode->connections[otherUserNode].first += weight;
		string field = userField;
		RemoveNewlines(field);
		userNode->connections[otherUserNode].second.insert(field);
	}
}


void Graph::CheckAndAddSimilarityFaculty(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
	const string& userField, const string& otherUserField, int weight, bool removeNewlines)
{
	if (!userField.empty() && !otherUserField.empty() && userField == otherUserField)
	{
		userNode->connections[otherUserNode].first += weight;
		string field = userField;
		if (removeNewlines)
			RemoveNewlines(field);
		userNode->connections[otherUserNode].second.insert(field);
	}
}

void Graph::CheckAndAddSimilaritySchool(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
	const vector<std::shared_ptr<School>>& userSchools, const vector<std::shared_ptr<School>>& otherUserSchools, int weight)
{
	if (!userSchools.empty() && !otherUserSchools.empty() && userSchools == otherUserSchools)
	{
		for (const auto& school : userSchools)
		{
			userNode->connections[otherUserNode].second.insert(school->name);
			userNode->connections[otherUserNode].first += weight;
		}
	}
}

void Graph::CheckAndAddCosineSimilarity(const std::shared_ptr<UserGraph>& userNode, const std::shared_ptr<UserGraph>& otherUserNode,
	const string& userField, const string& otherUserField, int weightMultiplier)
{
	if (!userField.empty() && !otherUserField.empty())
	{
		double similarity = CosineSimilarity(SplitIntoWords(userField), SplitIntoWords(otherUserField));
		if (similarity > 0.5)
		{
			userNode->connections[otherUserNode].first += static_cast<int>(similarity * weightMultiplier);
			userNode->connections[otherUserNode].second.insert(userField);
		}
	}
}

void Graph::RemoveIndirectConnections(vector<std::shared_ptr<UserGraph>>& users)
{
	vector<std::pair<std::shared_ptr<UserGraph>, std::shared_ptr<UserGraph>>> connectionsToRemove;

	for (const auto& userNode : users)
	{
		for (const auto& connection : userNode->connections)
		{
			const auto& otherUserNode = connection.first;

			for (const auto& indirectConnection : otherUserNode->connections)
			{
				const auto& indirectUserNode = indirectConnection.first;

				if (userNode->connections.find(indirectUserNode) != userNode->connections.end())
				{
					connectionsToRemove.emplace_back(otherUserNode, indirectUserNode);
				}
			}
		}
	}

	for (const auto& connectionToRemove : connectionsToRemove)
	{
		connectionToRemove.first->connections.erase(connectionToRemove.second);
	}
}

void Graph::SaveData(vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
	output << "{\n  \"nodes\": [\n";
	for (auto it = users.begin(); it != users.end(); ++it)
	{
		output << "    {\"id\": \"" << (*it)->user.id << "\"}";
		if (std::next(it) != users.end())
			output << ",";
		output << "\n";
	}
	output << "  ],\n  \"links\": [\n";

	for (auto it = users.begin(); it != users.end(); ++it)
	{
		const auto& userNode = *it;
		for (auto conn_it = userNode->connections.begin(); conn_it != userNode->connections.end(); ++conn_it)
		{
			output << "    {\"source\": \"" << userNode->user.id << "\", "
				   << "\"target\": \"" << conn_it->first->user.id << "\", "
				   << "\"weight\": \"" << conn_it->second.first << "\", "
				   << "\"similar\": \"" << GetSimilarParts(conn_it->second.second) << "\"}";
			if (std::next(conn_it) != userNode->connections.end() || std::next(it) != users.end())
				output << ",";
			output << "\n";
		}
	}
	output << "  ]\n}";
}

string Graph::GetSimilarParts(const std::unordered_set<string>& similarParts)
{
	string result;
	for (const auto& part : similarParts)
	{
		result += part + " ";
	}
	return result;
}
