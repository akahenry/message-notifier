#include <iostream>

#include <client.hpp>
#include <error.hpp>

int main(int argc, char **argv)
{
    Client *client;

    if (argc != 4)
    {
        std::cout << "Missing args. You must specify the username, the server address and its port (respectively).";
        return ERROR_MISSING_ARGS;
    }
    else
    {
        client = new Client(User(argv[1]), argv[2], argv[3]);
    }

    std::cout << "Username: " << argv[1] << std::endl;
    std::cout << "Server: " << argv[2] << std::endl;
    std::cout << "Port: " << argv[3] << std::endl;

    if (!client->connect())
    {
        return ERROR_CONNECTION_FAILED;
    }

    return 0;
}