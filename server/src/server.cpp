#include "server.hpp"

Server::Server(std::vector<User*> _users, std::string port)
{
    this->queue = new Queue<packet_item>();
    this->pkt_handler = new PacketHandler(this->queue);

    for (size_t i = 0; i < _users.size(); i++)
    {
        this->pkt_handler->addUser(_users[i]);
    }

    this->socket = Socket(port);
}

Server::Server(std::vector<User*> _users)
{
    this->queue = new Queue<packet_item>();
    this->pkt_handler = new PacketHandler(this->queue);

    for (size_t i = 0; i < _users.size(); i++)
    {
        this->pkt_handler->addUser(_users[i]);
    }
}

error_t Server::listen()
{
    this->running = true;
    this->pkt_handler->handle();
    return this->socket.configure(SOCKET_TYPE_SERVER);
}

error_t Server::listen(std::string port)
{
    this->socket = Socket(port);
    return this->listen();
}

error_t Server::serve()
{
    while(this->running)
    {
        Socket clientSocketPtr;
        error_t error = this->socket.serve(&clientSocketPtr);
        if (error)
        {
            std::cout << "Error accepting connection from client" << std::endl;
        }
        else
        {
            std::cout << "DEBUG: Client connected succesfully" << std::endl;
            Session* clientSession = new Session(clientSocketPtr, this->queue);
            auth_t authResponse = this->auth(clientSocketPtr);
            if(authResponse.isValid)
            {
                std::cout << "DEBUG: Checking for user" << std::endl;
                User* user;
                if (this->pkt_handler->hasUser(authResponse.username))
                {
                    user = this->pkt_handler->getUser(authResponse.username);
                    if (user->countSessions() >= 2)
                    {
                        std::cout << "INFO: User `" << authResponse.username << "` has exceed its sessions limit" << std::endl;
                        std::cout << "INFO: Connection refused for user " << authResponse.username << std::endl;
                        clientSession->close(ERROR_NOTIFICATION_UNAUTHORIZED);
                        delete clientSession;
                    }
                    else
                    {
                        std::cout << "DEBUG: Start listening user `" << authResponse.username << std::endl;
                        clientSession->listen();

                        std::cout << "DEBUG: Adding session for `" << authResponse.username << "`" << std::endl;
                        user->addSession(clientSession);

                        std::cout << "INFO: Connection accepted for user " << authResponse.username << std::endl;
                    }
                }
                else
                {
                    std::cout << "DEBUG: User `" << authResponse.username << "` not found" << std::endl;
                    user = new User(authResponse.username);

                    std::cout << "DEBUG: Start listening user `" << authResponse.username << std::endl;
                    clientSession->listen();

                    std::cout << "DEBUG: Adding session for `" << authResponse.username << "`" << std::endl;
                    user->addSession(clientSession);

                    this->pkt_handler->addUser(user);
                    std::cout << "INFO: Connection accepted for user " << authResponse.username << std::endl;
                }
            }
            else
            {
                delete clientSession;
            }
        }
    }

    return 0;
}


auth_t Server::auth(Socket socket)
{
    packet pkt;
    auth_t response = auth_t{"", false};

    std::cout << "DEBUG: Running authenticator thread" << std::endl;

    while(true)
    {
        if (socket.receive(&pkt))
        {
            return response;
        }

        if (pkt.type == PACKET_TYPE_CONNECT)
        {
            std::cout << "INFO: Connect request from user `" << pkt._username << "`" << std::endl;
            response.username = pkt._username;
            response.isValid = true;

            return response;
        }
    }
}