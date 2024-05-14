#include "Factory.h"
#include "fstream"

/*
void Graph::RemoveUser(int userId) {
    users.erase(std::remove_if(users.begin(), users.end(), [userId](const auto& userNode) {
        return userNode->user.id == userId;
        }), users.end());
}
*/

void Graph::AddConnectionsInSameCity(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{

    for (const auto& userNode : users)
    {
        for (const auto& otherUserNode : users)
        {

            if (!userNode->user.city.empty() && userNode != otherUserNode && userNode->user.city == otherUserNode->user.city)
            {
                userNode->connections[otherUserNode] += 2;
            }
            if (!userNode->user.interests.empty() && userNode != otherUserNode && userNode->user.interests == otherUserNode->user.interests)
            {
                userNode->connections[otherUserNode] += 4;
            }
            if (!userNode->user.activities.empty() && userNode != otherUserNode && userNode->user.activities == otherUserNode->user.activities)
            {
                userNode->connections[otherUserNode] += 3;
            }
            if (!userNode->user.education.empty() && userNode != otherUserNode && userNode->user.education == otherUserNode->user.education)
            {
                userNode->connections[otherUserNode] += 3;
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
    }
}

void Graph::SaveData(std::vector<std::shared_ptr<UserGraph>>& users, std::ofstream& output)
{
    for (const auto& userNode : users)
    {
        output << "id: " << userNode->user.id << std::endl;
        for (const auto& connection : userNode->connections) {
            output << "ConnectionId: " << connection.first->user.id << "\nvalue: " << connection.second << std::endl;
        }
        output << "end" << std::endl;

    }
}