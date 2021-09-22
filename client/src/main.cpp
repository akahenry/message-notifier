#include <iostream>

#include "client.hpp"
#include "error.hpp"

int main(int argc, char **argv)
{
    Client *client;

    if (argc < 3)
    {
        std::cout << "Missing args. You must specify the username, the server address and its port (respectively).";
        return ERROR_MISSING_ARGS;
    }
    else
    {
        std::string username;
        std::string hostname;
        std::string port;

        if (argc == 3)
        {
            std::string username;
            std::cout << "Enter your username: ";
            std::cin >> username;
            hostname = argv[1];
            port = argv[2];
        } 
        else 
        {
            username = argv[1];
            hostname = argv[2];
            port = argv[3];
        }

        client = new Client(username, hostname, port);
    }

    if (client->connect())
    {
        std::cout << "Error configurating client. Exiting..." << std::endl;
        return ERROR_CONFIGURATION_FAILED;
    }

    while (true) 
    {
        std::string input;
        std::cin >> input;
        client->send(PACKET_TYPE_DATA, input);
    }

    return 0;
}