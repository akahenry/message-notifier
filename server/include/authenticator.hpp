#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <thread>
#include <iostream>

#include "models.hpp"
#include "socket.hpp"

typedef struct _auth_t
{
    std::string username;
    bool isValid;
} auth_t;

class Authenticator
{
    private:
        Socket socket;

    public:
        Authenticator(Socket _socket);
        auth_t authenticate();
};

#endif