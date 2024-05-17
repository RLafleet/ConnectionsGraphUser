#include "Factory.h"
#include "Controller.h"
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

std::string UTF8_to_CP1251(std::string const& utf8)
{
    if (!utf8.empty())
    {
        int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
        if (wchlen > 0 && wchlen != 0xFFFD)
        {
            std::vector<wchar_t> wbuf(wchlen);
            MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
            std::vector<char> buf(wchlen);
            WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);

            return std::string(&buf[0], wchlen);
        }
    }
    return std::string();
}


std::unordered_set<std::string> splitIntoWords(const std::string& str) {
    std::unordered_set<std::string> words;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        words.insert(word);
    }
    return words;
}

bool hasSimilarWords(const std::string& str1, const std::string& str2) {
    const size_t minWordLength = 4;
    std::unordered_set<std::string> words1 = splitIntoWords(str1);
    std::unordered_set<std::string> words2 = splitIntoWords(str2);

    size_t count = 0;
    size_t maxCount = min(words1.size(), static_cast<size_t>(6));
    for (const auto& word1 : words1) {
        if (word1.size() >= minWordLength) {
            auto it = words2.find(word1);
            if (it != words2.end() && it->size() >= minWordLength) {
                count++;
                if (count >= maxCount) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Graph::AddConnections(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    // Создаем хэш-таблицы для быстрого доступа к пользователям по атрибутам
    /*
    std::unordered_map<std::string, std::vector<std::shared_ptr<UserGraph>>> cityMap;
    std::unordered_map<std::string, std::vector<std::shared_ptr<UserGraph>>> interestsMap;
    std::unordered_map<std::string, std::vector<std::shared_ptr<UserGraph>>> activitiesMap;
    std::unordered_map<std::string, std::vector<std::shared_ptr<UserGraph>>> educationMap;

    for (const auto& user : users)
    {
        if (!user->user.city.empty())
            cityMap[user->user.city].push_back(user);
        if (!user->user.interests.empty())
            interestsMap[user->user.interests].push_back(user);
        if (!user->user.activities.empty())
            activitiesMap[user->user.activities].push_back(user);
        if (!user->user.education.empty())
            educationMap[user->user.education].push_back(user);
    }
    */
    for (size_t i = 0; i < users.size(); ++i)
    {
        const auto& userNode = users[i];
        std::vector<bool> visited(users.size(), false);

        for (size_t j = i + 1; j < users.size(); ++j)
        {
            if(!visited[j])
            {
                const auto& otherUserNode = users[j];   
                if ((!userNode->user.city.empty() && userNode->user.city == otherUserNode->user.city)) {
                    userNode->connections[otherUserNode] += 1;
                }
                if ((!userNode->user.homeTown.empty() && userNode->user.homeTown == otherUserNode->user.homeTown)) {
                    userNode->connections[otherUserNode] += 1;
                }
                if ((!userNode->user.schools.empty() && userNode->user.schools == otherUserNode->user.schools)) {
                    userNode->connections[otherUserNode] += 4;
                }
                if ((!userNode->user.education.empty() && userNode->user.education == otherUserNode->user.education)) {
                    userNode->connections[otherUserNode] += 3;
                }
            
                if ((!userNode->user.interests.empty() && hasSimilarWords(userNode->user.interests, otherUserNode->user.interests))) {
                    userNode->connections[otherUserNode] += 4;
                }

                if ((!userNode->user.activities.empty() && hasSimilarWords(userNode->user.activities, otherUserNode->user.activities))) {
                    userNode->connections[otherUserNode] += 5;
                }

                if ((!userNode->user.about.empty() && hasSimilarWords(userNode->user.about, otherUserNode->user.about))) {
                    userNode->connections[otherUserNode] += 5;
                }

                if ((!userNode->user.movies.empty() && hasSimilarWords(userNode->user.movies, otherUserNode->user.movies))) {
                    userNode->connections[otherUserNode] += 3;
                }

                if ((!userNode->user.books.empty() && hasSimilarWords(userNode->user.books, otherUserNode->user.books))) {
                    userNode->connections[otherUserNode] += 5;
                }
                visited[j] = true;
            }
        }
    }

    std::vector<std::pair<std::shared_ptr<UserGraph>, std::shared_ptr<UserGraph>>> connectionsToRemove;

    for (size_t i = 0; i < users.size(); ++i)
    {
        const auto& userNode = users[i];

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


void Graph::SaveData(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    output << "{" << std::endl;
    output << "  \"nodes\": [" << std::endl;
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        const auto& userNode = *it;
        output << "    {\"id\": \"" << userNode->user.id << "\"}";

        if (std::next(it) != users.end())
        {
            output << ",";
        }

        output << std::endl;
    }
    output << "  ]," << std::endl;
    output << "  \"links\": [" << std::endl;
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        const auto& userNode = *it;
        for (auto conn_it = userNode->connections.begin(); conn_it != userNode->connections.end(); ++conn_it)
        {
            const auto& connection = *conn_it;
            output << "    {\"source\": \"" << userNode->user.id << "\", ";
            output << "\"target\": \"" << connection.first->user.id << "\", ";
            output << "\"weight\": \"" << connection.second << "\"}";

            if (std::next(conn_it) != userNode->connections.end() || std::next(it) != users.end())
            {
                output << ","; 
            }

            output << std::endl;
        }
    }

    output << "  ]" << std::endl;
    output << "}" << std::endl;
}