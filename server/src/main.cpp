#include "server.hpp"

int main(int argc, char **argv)
{
    Server* server;
    std::string port;
    std::vector<User*> users;

    if (argc == 2)
    {
        port = argv[1];
    } 
    else 
    {
        std::cout << "Enter the port number: ";
        std::cin >> port;
    }

    server = new Server(users, port);

    if (server->listen())
    {
        std::cout << "Error configurating server. Exiting..." << std::endl;
        return ERROR_CONFIGURATION_FAILED;
    }

    server->serve();

    return 0;
}