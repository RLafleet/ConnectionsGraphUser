#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>

struct School {
    int city;
    std::string class_;
    int class_id;
    int country;
    std::string id;
    std::string name;
    int year_from;
    int year_graduated;
    int year_to;
    std::string speciality;
};

struct Relative {
    std::string type;
    int id;
    std::string birth_date;
    std::string name;
};

struct UserData {
    int id = 0;
    std::string firstName;
    std::string lastName;
    bool canAccessClosed;
    std::string interests = "";
    std::string books = "";
    std::string about = "";
    std::string movies = "";
    std::string activities = "";
    std::string music = "";
    std::string education = "";
    std::string homeTown = "";
    int relation = -1;
    std::vector<std::shared_ptr<School>> schools;
    std::vector<std::shared_ptr<Relative>> relatives;
    std::string city = "";
    std::string country = "";
};

#endif //USER_H