#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "user.hpp"

class Client : public User<packet>
{
    using User::User;
    
    protected:
        void onStopListener();
        void onStopPublisher();
};

#endif