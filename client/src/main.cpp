#include <iostream>
#include <csignal>

#include "client.hpp"
#include "error.hpp"
#include "logger.hpp"

Client *client;

void signalHandler(int signum) {
    log(LOG_TYPE_DEBUG, "Interrupt signal (" + std::to_string(signum) + ") received. Exiting...");

    client->send(PACKET_TYPE_CMD_EXIT, "");
    client->close();

    exit(signum);  
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        log(LOG_TYPE_INFO, "Missing args. You must specify the username, the server address and its port (respectively).");
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
         log(LOG_TYPE_INFO, "Error configurating client. Exiting...");
        return ERROR_CONFIGURATION_FAILED;
    }

    std::string input;
    while (std::getline(std::cin, input)) 
    {
        if (std::cin.eof())
        {
            log(LOG_TYPE_DEBUG,  "Exiting from client...");
            client->send(PACKET_TYPE_CMD_EXIT, "");
            client->close();
            break;
        }
        std::cout << "> ";

        log(LOG_TYPE_DEBUG,  "Input `" + input + "` from user");
        if (input.rfind("FOLLOW @", 0) == 0)
        {
            log(LOG_TYPE_DEBUG,  "Sending follow `" + std::string(&input[8]) + "` to client");
            client->send(PACKET_TYPE_CMD_FOLLOW, &input[8]);
        }
        else if (input.rfind("SEND ", 0) == 0)
        {
            log(LOG_TYPE_DEBUG,  "Sending message `" + std::string(&input[5]) + "` to client");
            client->send(PACKET_TYPE_CMD_SEND, &input[5]);
        }
        else if (input.rfind("EXIT", 0) == 0)
        {
            log(LOG_TYPE_DEBUG,  "Exiting from client...");
            client->send(PACKET_TYPE_CMD_EXIT, "");
            client->close();
            break;
        }
    }

    return 0;
}