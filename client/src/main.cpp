#include <iostream>
#include <csignal>

#include "client.hpp"
#include "error.hpp"

Client *client;

void signalHandler(int signum) {
    std::cout << "\nDEBUG: Interrupt signal (" << signum << ") received. Exiting...\n";

    client->send(PACKET_TYPE_CMD_EXIT, "");
    client->close();

    exit(signum);  
}

int main(int argc, char **argv)
{
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
        std::signal(SIGINT, signalHandler);
    }

    if (client->connect())
    {
        std::cout << "Error configurating client. Exiting..." << std::endl;
        return ERROR_CONFIGURATION_FAILED;
    }

    while (true) 
    {
        std::string input;
        std::getline(std::cin, input);
        std::cout << "DEBUG: Input `" << input << "` from user" << std::endl;
        if (input.rfind("FOLLOW @", 0) == 0)
        {
            std::cout << "DEBUG: Sending follow `" << &input[8] << "` to client" << std::endl;
            client->send(PACKET_TYPE_CMD_FOLLOW, &input[8]);
        }
        else if (input.rfind("SEND ", 0) == 0)
        {
            std::cout << "DEBUG: Sending message `" << &input[5] << "` to client" << std::endl;
            client->send(PACKET_TYPE_CMD_SEND, &input[5]);
        }
        else if (input.rfind("EXIT", 0) == 0)
        {
            std::cout << "DEBUG: Exiting from client..." << std::endl;
            client->send(PACKET_TYPE_CMD_EXIT, "");
            client->close();
            break;
        }
    }

    return 0;
}