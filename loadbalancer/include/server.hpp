#ifndef SERVER_HPP
#define SERVER_HPP

#include "user.hpp"

class Server : public User<notification>
{
    using User::User;

    protected:
        void onStopListener();
        void onStopPublisher();
};

#endif