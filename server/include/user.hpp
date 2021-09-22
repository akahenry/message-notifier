#include <string>
#include <vector>
#include <mutex>

#include "session.hpp"

class User
{
    private:
        std::string name;
        std::vector<User*> following;
        std::vector<User*> followers;
        std::vector<Session*> sessions;
        std::mutex mutex;

    public:
        User();
        User(std::string _name);
        User(std::string _name, std::vector<User*> _following, std::vector<User*> _followers);

        void lock();
        void unlock();

        void addFollower(User* user);
        void follow(User* user);
        std::vector<User*> getFollowers();
        std::string getName();
        void addSession(Session* session);
        bool removeSession(Session* session);
        int countSessions();
};