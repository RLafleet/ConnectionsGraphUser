#include "Controller.h"
using json = nlohmann::json;

Controller::Controller() {}

Controller::Controller(const std::string& filename) 
{
    ReadUserDataFromFile(filename);
}

std::string Controller::UTF8_to_CP1251(std::string const& utf8)
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

void Controller::ReadUserDataFromFile(const std::string& filename) {
    std::ifstream file("data.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }


    std::string json_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::vector<json> data = json::parse(json_str);


    for (const auto& obj : data) 
    {
        auto user = std::make_shared<UserData>();
        auto graphVerticle = std::make_shared<Graph>();
        bool ok = false;

        user->id = obj["id"];
        user->canAccessClosed = obj["can_access_closed"];
        user->firstName = obj["first_name"];
        user->lastName = obj["last_name"];
        if (obj.find("interests") != obj.end()) 
        {
            ok = true;
            user->interests = obj["interests"];
        }
        if (obj.find("books") != obj.end()) 
        {
            ok = true;
            user->books = obj["books"];
        }
        if (obj.find("about") != obj.end()) 
        {
            ok = true;
            user->about = obj["about"];
        }
        if (obj.find("movies") != obj.end()) 
        {
            ok = true;
            user->movies = obj["movies"];
        }
        if (obj.find("activities") != obj.end()) 
        {
            ok = true;
            user->activities = obj["activities"];
        }
        if (obj.find("music") != obj.end()) 
        {
            ok = true;
            user->music = obj["music"];
        }
        if (obj.find("university_name") != obj.end()) 
        {
            ok = true;
            user->education = obj["university_name"];
        }
        if (obj.find("home_town") != obj.end()) 
        {
            ok = true;
            user->homeTown = obj["home_town"];
        }
        if (obj.find("relation") != obj.end()) 
        {
            ok = true;
            user->relation = obj["relation"];
        }
        if (obj.find("city") != obj.end() && obj["city"].find("title") != obj["city"].end()) {
            ok = true;
            user->city = obj["city"]["title"];
        }
        if (obj.find("country") != obj.end() && obj["country"].find("title") != obj["country"].end()) {
            ok = true;
            user->country = obj["country"]["title"];
        }

        if (obj.find("schools") != obj.end()) {
            for (const auto& schoolJson : obj["schools"]) {
                auto school = std::make_shared<School>();

                if (schoolJson.find("city") != schoolJson.end()) {
                    ok = true;
                    school->city = schoolJson["city"];
                }

                if (schoolJson.find("class") != schoolJson.end()) {
                    ok = true;
                    school->class_ = schoolJson["class"];
                }

                if (schoolJson.find("class_id") != schoolJson.end()) {
                    ok = true;
                    school->class_id = schoolJson["class_id"];
                }

                if (schoolJson.find("country") != schoolJson.end()) {
                    ok = true;
                    school->country = schoolJson["country"];
                }

                if (schoolJson.find("id") != schoolJson.end()) {
                    ok = true;
                    school->id = schoolJson["id"];
                }

                if (schoolJson.find("name") != schoolJson.end()) {
                    ok = true;
                    school->name = schoolJson["name"];
                }

                if (schoolJson.find("year_from") != schoolJson.end()) {
                    ok = true;
                    school->year_from = schoolJson["year_from"];
                }

                if (schoolJson.find("year_graduated") != schoolJson.end()) {
                    ok = true;
                    school->year_graduated = schoolJson["year_graduated"];
                }

                if (schoolJson.find("year_to") != schoolJson.end()) {
                    ok = true;
                    school->year_to = schoolJson["year_to"];
                }

                if (schoolJson.find("speciality") != schoolJson.end()) {
                    ok = true;
                    school->speciality = schoolJson["speciality"];
                }

                user->schools.push_back(school);
            }
        }   
        if (obj.find("relatives") != obj.end()) {
            for (const auto& relativeJson : obj["relatives"]) {
                auto relative = std::make_shared<Relative>();

                if (relativeJson.find("type") != relativeJson.end()) {
                    ok = true;
                    relative->type = relativeJson["type"];
                }

                if (relativeJson.find("id") != relativeJson.end()) {
                    ok = true;
                    relative->id = relativeJson["id"];
                }

                if (relativeJson.find("birth_date") != relativeJson.end()) {
                    ok = true;
                    relative->birth_date = relativeJson["birth_date"];
                }

                if (relativeJson.find("name") != relativeJson.end()) {
                    ok = true;
                    relative->name = relativeJson["name"];
                }

                user->relatives.push_back(relative);
            }
        }

        if (ok)
        {
            AddUser(user);
        }

    }

    std::ofstream output("output.txt");
    if (!output.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    Graph graph;
    graph.AddConnectionsInSameCity(users, output);
    graph.SaveData(users, output);

    file.close();


}


void Controller::AddUser(const std::shared_ptr<UserData>& userData) {
    auto userNode = std::make_shared<UserGraph>();
    userNode->user = *userData;
    users.push_back(userNode);
}

void Controller::PrintUsers() 
{

    
}