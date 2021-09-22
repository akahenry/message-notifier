#include <user.hpp>

User::User(std::string _name) : name{_name} {}

User::User(std::string _name, std::vector<User*> _following, std::vector<User*> _followers) : name{_name}, following{_following}, followers{_followers} {}

void User::addFollower(User* user) 
{
    this->mutex.lock();
    this->followers.push_back(user);
    this->mutex.unlock();
}

void User::follow(User* user) 
{
    this->mutex.lock();
    user->addFollower(this);
    this->following.push_back(user);
    this->mutex.unlock();
}

std::string User::getName()
{
    return this->name;
}

void User::lock()
{
    this->mutex.lock();
}

void User::unlock()
{
    this->mutex.unlock();
}

void User::addSession(Session* session)
{
    this->mutex.lock();
    this->sessions.push_back(session);
    this->mutex.unlock();
}

bool User::removeSession(Session* session)
{
    bool deleted = false;

    this->mutex.lock();
    for (size_t i = 0; i < this->sessions.size(); i++)
    {
        if (this->sessions[i] == session)
        {
            this->sessions.erase(this->sessions.begin() + i);
            deleted = true;
            break;
        }
    }
    this->mutex.unlock();

    return deleted;
}

int User::countSessions()
{
    this->mutex.lock();
    return this->sessions.size();
    this->mutex.unlock();
}