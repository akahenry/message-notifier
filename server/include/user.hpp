#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <mutex>
#include <iostream>

#include "session.hpp"
#include "queue.hpp"

class User
{
    private:
        std::string name;
        std::vector<User*> following;
        std::vector<User*> followers;
        std::vector<Session*> sessions;
        Queue<notification> pending;
        std::mutex mutex;

        void publish();
    public:
        User();
        User(std::string _name);
        User(std::string _name, std::vector<User*> _following, std::vector<User*> _followers, Queue<notification> _pending);

        void lock();
        void unlock();

        void addFollower(User* user);
        void follow(User* user);
        std::vector<User*> getFollowers();
        std::vector<User*> getFollowing();
        std::vector<notification> getPending();
        std::string getName();
        void addSession(Session* session);
        void removeSession(Session* session);
        void removeSession(uint16_t session_id);
        std::vector<Session*> getSessions();
        int countSessions();    
        void pushNotification(notification notif);
};

#endif