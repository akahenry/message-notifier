#include <map>

#include "user.hpp"

class Server
{
    private:
        std::map<std::string, User> users;

    public:
        Server();
};