#include "Factory.h"
#include "fstream"

/*
void Graph::RemoveUser(int userId) {
    users.erase(std::remove_if(users.begin(), users.end(), [userId](const auto& userNode) {
        return userNode->user.id == userId;
        }), users.end());
}
*/

void Graph::AddConnections(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    std::vector<bool> visited(users.size(), false); // Initialize visited vector

    for (size_t i = 0; i < users.size(); ++i)
    {
        if (visited[i]) // If the user has been visited, skip
            continue;

        const auto& userNode = users[i];

        for (size_t j = i + 1; j < users.size(); ++j)
        {
            if (visited[j]) // If the other user has been visited, skip
                continue;

            const auto& otherUserNode = users[j];

            if (!userNode->user.city.empty() && userNode->user.city == otherUserNode->user.city)
            {
                userNode->connections[otherUserNode] += 2;
                otherUserNode->connections[userNode] += 2;
            }
            if (!userNode->user.interests.empty() && userNode->user.interests == otherUserNode->user.interests)
            {
                otherUserNode->connections[userNode] += 4;
                userNode->connections[otherUserNode] += 4;
            }
            if (!userNode->user.activities.empty() && userNode->user.activities == otherUserNode->user.activities)
            {
                otherUserNode->connections[userNode] += 3;
                userNode->connections[otherUserNode] += 3;
            }
            if (!userNode->user.education.empty() && userNode->user.education == otherUserNode->user.education)
            {
                otherUserNode->connections[userNode] += 3;
                userNode->connections[otherUserNode] += 3;
            }
        }

        visited[i] = true; // Mark the current user as visited
    }
    /*
    if (!userNode->user.country.empty() && userNode != otherUserNode && userNode->user.country == otherUserNode->user.country)
    {
        userNode->connections[otherUserNode] += 1;
    }
    if (!userNode->user.homeTown.empty() && userNode != otherUserNode && userNode->user.homeTown == otherUserNode->user.homeTown)
    {
        userNode->connections[otherUserNode] += 1;
    }
    */

}

void Graph::SaveData(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    for (const auto& userNode : users)
    {
        output << userNode->user.id << std::endl;
        for (const auto& connection : userNode->connections) {
            output << connection.first->user.id << "\n" << connection.second << std::endl;
        }
        output << "end" << std::endl;

    }
}