#include <user.hpp>

User::User(std::string _name) : name{_name} {}

User::User(std::string _name, std::vector<User> _following, std::vector<User> _followers) : name{_name}, following{_following}, followers{_followers} {}

void User::addFollower(User user) 
{
    this->followers.push_back(user);
}

void User::follow(User user) 
{
    user.addFollower(*this);
    this->following.push_back(user);
}

std::string User::getName()
{
    return this->name;
}