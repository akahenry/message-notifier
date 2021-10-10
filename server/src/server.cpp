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
            log(LOG_TYPE_INFO, "Error accepting connection from client");
        }
        else
        {
            log(LOG_TYPE_DEBUG,  "Client connected succesfully");
            Session* clientSession = new Session(clientSocketPtr, this->queue);
            auth_t authResponse = this->auth(clientSocketPtr);
            if(authResponse.isValid)
            {
                log(LOG_TYPE_DEBUG,  "Checking for user");
                User* user;
                if (this->pkt_handler->hasUser(authResponse.username))
                {
                    user = this->pkt_handler->getUser(authResponse.username);
                    if (user->countSessions() >= 2)
                    {
                        log(LOG_TYPE_INFO,  "User `" + authResponse.username + "` has exceed its sessions limit");
                        log(LOG_TYPE_INFO,  "Connection refused for user " + authResponse.username);
                        clientSession->close(ERROR_NOTIFICATION_UNAUTHORIZED);
                        delete clientSession;
                    }
                    else
                    {
                        log(LOG_TYPE_DEBUG,  "Start listening user `" + authResponse.username);
                        clientSession->listen();

                        log(LOG_TYPE_DEBUG,  "Adding session for `" + authResponse.username + "`");
                        user->addSession(clientSession);

                        log(LOG_TYPE_INFO,  "Connection accepted for user " + authResponse.username);
                    }
                }
                else
                {
                    log(LOG_TYPE_DEBUG,  "User `" + authResponse.username + "` not found");
                    user = new User(authResponse.username);

                    log(LOG_TYPE_DEBUG,  "Start listening user `" + authResponse.username);
                    clientSession->listen();

                    log(LOG_TYPE_DEBUG,  "Adding session for `" + authResponse.username + "`");
                    user->addSession(clientSession);

                    this->pkt_handler->addUser(user);
                    log(LOG_TYPE_INFO,  "Connection accepted for user " + authResponse.username);
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

    log(LOG_TYPE_DEBUG,  "Running authenticator thread");

    while(true)
    {
        if (socket.receive(&pkt))
        {
            return response;
        }

        if (pkt.type == PACKET_TYPE_CONNECT)
        {
            log(LOG_TYPE_INFO,  "Connect request from user `" + std::string(pkt._username) + "`");
            response.username = pkt._username;
            response.isValid = true;

            return response;
        }
    }
}

error_t Server::close()
{
    this->pkt_handler->close();

    return 0;
}

error_t Server::save(std::string path)
{
    this->pkt_handler->save(path);

    return 0;
}