#include "Factory.h"
#include "Controller.h"

using std::string;
using std::vector;

string Graph::UTF8_to_CP1251(string const& utf8)
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

            return string(&buf[0], wchlen);
        }
    }
    return string();
}

string Graph::ToLower(const string& str) {
    string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string Graph::NormalizeString(const string& str) {
    string result;
    for (char ch : str) {
        if (ch == '-') {
            result += ' ';
        }
        else {
            result += ch;
        }
    }
    
    return result;
}

std::unordered_set<string> Graph::SplitIntoWords(const string& str) {
    std::unordered_set<string> words;
    string normalizedStr = NormalizeString(str);
    std::istringstream iss(normalizedStr);
    string word;
    while (iss >> word) {
        words.insert(ToLower(word));
    }
    return words;
}

bool Graph::HasSimilarWords(const string& str1, const string& str2, string& similar) {
    const size_t minWordLength = 4;
    std::unordered_set<string> words1 = SplitIntoWords(str1);
    std::unordered_set<string> words2 = SplitIntoWords(str2);

    size_t count = 0;
    size_t maxCount = min(words1.size(), static_cast<size_t>(6));
    for (const auto& word1 : words1) 
    {
        if (word1.size() >= minWordLength) 
        {
            auto it = words2.find(word1);
            if (it != words2.end() && it->size() >= minWordLength) 
            {
                count++;
                similar += word1;
                if (count >= maxCount) 
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Graph::AddConnections(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    for (size_t i = 0; i < users.size(); ++i)
    {
        const auto& userNode = users[i];
        std::vector<bool> visited(users.size(), false);

        for (size_t j = i + 1; j < users.size(); ++j)
        {
            if(!visited[j])
            {
                const auto& otherUserNode = users[j];   
                string similar = "";

                const string userCity = userNode->user.city;
                const string otherUserCity = otherUserNode->user.city;

                const string userHomeTown = userNode->user.homeTown;
                const string otherUserHomeTown = otherUserNode->user.homeTown;

                std::vector<std::string> userSchoolNames;
                for (const auto& school : userNode->user.schools) {
                    userSchoolNames.push_back(school->name);
                }
                std::vector<std::string> otherUserSchoolNames;
                for (const auto& school : otherUserNode->user.schools) {
                    otherUserSchoolNames.push_back(school->name);
                }

                const string userEducation = userNode->user.education;
                const string otherUserEducation = otherUserNode->user.education;

                const string userInterests = userNode->user.interests;
                const string otherUserInterests = otherUserNode->user.interests;

                const string userActivities = userNode->user.activities;
                const string otherUserActivities = otherUserNode->user.activities;

                const string userAbout = userNode->user.about;
                const string otherUserAbout = otherUserNode->user.about;

                const string userMovies = userNode->user.movies;
                const string otherUserMovies = otherUserNode->user.movies;

                const string userBooks = userNode->user.books;
                const string otherUserBooks = otherUserNode->user.books;

                if ((!userCity.empty() && userCity == otherUserCity)) {
                    userNode->connections[otherUserNode].first += 1;
                    userNode->connections[otherUserNode].second.insert(userCity);
                }
                if ((!userHomeTown.empty() && userHomeTown == otherUserHomeTown)) {
                    userNode->connections[otherUserNode].first += 1;
                    userNode->connections[otherUserNode].second.insert(userHomeTown);
                }

                if ((!userSchoolNames.empty() && userSchoolNames == otherUserSchoolNames)) {
                    for (const auto& schoolNames : userSchoolNames)
                    {
                        userNode->connections[otherUserNode].second.insert(schoolNames);
                        userNode->connections[otherUserNode].first += 4;
                    }
                }

                if ((!userEducation.empty() && userEducation == otherUserEducation)) {
                    userNode->connections[otherUserNode].first += 3;
                    userNode->connections[otherUserNode].second.insert(userEducation);
                }

                if (!userInterests.empty() && HasSimilarWords(userInterests, otherUserInterests, similar)) {
                    userNode->connections[otherUserNode].first += 4;
                    userNode->connections[otherUserNode].second.insert(similar);
                }

                if (!userActivities.empty() && HasSimilarWords(userActivities, otherUserActivities, similar)) {
                    userNode->connections[otherUserNode].first += 5;
                    userNode->connections[otherUserNode].second.insert(similar);
                }

                if (!userAbout.empty() && HasSimilarWords(userAbout, otherUserAbout, similar)) {
                    userNode->connections[otherUserNode].first += 5;
                    userNode->connections[otherUserNode].second.insert(similar);
                }

                if (!userMovies.empty() && HasSimilarWords(userMovies, otherUserMovies, similar)) {
                    userNode->connections[otherUserNode].first += 3;
                    userNode->connections[otherUserNode].second.insert(similar);
                }

                if (!userBooks.empty() && HasSimilarWords(userBooks, otherUserBooks, similar)) {
                    userNode->connections[otherUserNode].first += 5;
                    userNode->connections[otherUserNode].second.insert(similar);
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
            output << "\"weight\": \"" << connection.second.first << "\", ";
            output << "\"similar\": \"";
            for (const auto& similarParts : connection.second.second)
            {
                output << similarParts << " ";
            }
            output << "\"}";

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