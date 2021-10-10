#include <user.hpp>

User::User(std::string _name) : name{_name} {}

User::User(std::string _name, std::vector<User*> _following, std::vector<User*> _followers, Queue<notification> _pending) : name{_name}, following{_following}, followers{_followers}, pending{_pending} {}

void User::addFollower(User* user) 
{
    bool found = false;
    this->mutex.lock();
    for (size_t i = 0; i < this->followers.size(); i++)
    {
        if (this->followers[i] == user)
        {
            found = true;
        }
    }

    if (!found)
    {
        this->followers.push_back(user);
    }
    this->mutex.unlock();
}

void User::follow(User* user) 
{
    bool found = false;
    user->addFollower(this);
    
    this->mutex.lock();
    for (size_t i = 0; i < this->following.size(); i++)
    {
        if (this->following[i] == user)
        {
            found = true;
        }
    }

    if (!found)
    {
        this->following.push_back(user);
    }
    this->mutex.unlock();
}

std::string User::getName()
{
    return this->name;
}

std::vector<User*> User::getFollowers()
{
    return this->followers;
}

std::vector<User*> User::getFollowing()
{
    return this->following;
}

std::vector<notification> User::getPending()
{
    return this->pending.data();
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
    log(LOG_TYPE_DEBUG,  "Trying to lock user");
    this->mutex.lock();
    session->id = this->sessions.size();
    this->sessions.push_back(session);
    this->mutex.unlock();
    log(LOG_TYPE_DEBUG,  "Trying to unlock user");
    this->publish();
}

void User::removeSession(Session* session)
{
    this->mutex.lock();
    this->sessions.erase(this->sessions.begin() + session->id);
    session->finish();
    this->mutex.unlock();
}

void User::removeSession(uint16_t session_id)
{
    this->mutex.lock();
    Session* session = this->sessions[session_id];
    this->mutex.unlock();
    this->removeSession(session);
}

std::vector<Session*> User::getSessions()
{
    return this->sessions;
}

int User::countSessions()
{
    this->mutex.lock();
    int size = this->sessions.size();
    this->mutex.unlock();

    return size;
}

void User::pushNotification(notification notif)
{
    this->pending.push(notif);
    this->publish();
}

void User::publish()
{
    notification notif;
    if (this->sessions.size() > 0)
    {
        while (!this->pending.empty())
        {
            notif = this->pending.pop();
            for (size_t i = 0; i < this->sessions.size(); i++)
            {
                this->sessions[i]->send(MESSAGE_TYPE_NOTIFICATION, (void*)&notif);
            }
        }
    }
}