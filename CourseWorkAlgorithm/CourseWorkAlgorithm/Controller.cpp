#include "Controller.h"
using json = nlohmann::json;

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
    std::ifstream file(filename);
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
            user->interests = obj["interests"];
            if (!user->interests.empty())
            {
                ok = true;
            }
        }
        if (obj.find("about") != obj.end())
        {
            user->about = obj["about"];
            if (!user->about.empty())
            {
                ok = true;
            }
        }
        if (obj.find("books") != obj.end())
        {
            user->books = obj["books"];
            if (!user->books.empty())
            {
                ok = true;
            }
        }
        if (obj.find("movies") != obj.end())
        {
            user->movies = obj["movies"];
            if (!user->movies.empty())
            {
                ok = true;
            }
        }
        if (obj.find("faculty_name") != obj.end() && !obj["faculty_name"].empty())
        {
            user->education = obj["faculty_name"];
            if (!user->education.empty())
            {
                ok = true;
                //std::cout << UTF8_to_CP1251(user->education) << std::endl;
            }
        }

        if (obj.find("activities") != obj.end() && !obj["activities"].empty())
        {
            user->activities = obj["activities"];
            if (!user->activities.empty())
            {
                ok = true;
                //std::cout << UTF8_to_CP1251(user->activities) << std::endl;
            }
        }
        /*
        if (obj.find("home_town") != obj.end())
        {
            user->homeTown = obj["home_town"];
            if (!user->homeTown.empty())
            {
                ok = true;
            }
        }
        if (obj.find("city") != obj.end())
        {
            user->city = obj["city"];
            if (!user->city.empty())
            {
                ok = true;
            }
        }
        */
        if (obj.find("schools") != obj.end()) {
            for (const auto& schoolJson : obj["schools"]) {
                auto school = std::make_shared<School>();
                if (schoolJson.find("id") != schoolJson.end()) {
                    school->id = schoolJson["id"];
                    if (!school->id.empty())
                    {
                        ok = true;
                    }
                }

                if (schoolJson.find("name") != schoolJson.end()) {
                    school->name = schoolJson["name"];
                    if (!school->name.empty())
                    {
                        ok = true;
                    }
                }
                /*
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
                */
                user->schools.push_back(school);
            }
        }
        /*

        if (obj.find("university_name") != obj.end() && !obj["university_name"].empty())
        {
            user->education = obj["university_name"];
            if (!user->education.empty())
            {
                ok = true;
                //std::cout << UTF8_to_CP1251(user->education) << std::endl;
            }
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
          if (obj.find("country") != obj.end() && obj["country"].find("title") != obj["country"].end()) {
            ok = true;
            user->country = obj["country"]["title"];
        }
        if (obj.find("city") != obj.end() && obj["city"].find("title") != obj["city"].end() && !obj["city"]["title"].empty()) {
            
            user->city = obj["city"]["title"];
            if (!user->city.empty())
            {
                ok = true;
                //std::cout << UTF8_to_CP1251(user->city) << std::endl;
            }
        }
        if (obj.find("schools") != obj.end()) {
            for (const auto& schoolJson : obj["schools"]) {
                auto school = std::make_shared<School>();

                if (schoolJson.find("city") != schoolJson.end()) {
                    school->city = schoolJson["city"];
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
        */

        if (ok)
        {
            AddUser(user);
        }

    }

    std::ofstream output("output.json");
    if (!output.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    Graph graph;
    std::cout << users.size() << std::endl;
    graph.AddConnections(users, output);
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