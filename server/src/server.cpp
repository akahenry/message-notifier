#include "server.hpp"

Server::Server(std::vector<User*> _users, std::string port)
{
    this->users_mutex.lock();
    for (size_t i = 0; i < _users.size(); i++)
    {
        this->users.insert({_users[i]->getName(), _users[i]});
    }
    this->users_mutex.unlock();

    this->socket = Socket(port);
}

Server::Server(std::vector<User*> _users)
{
    this->users_mutex.lock();
    for (size_t i = 0; i < _users.size(); i++)
    {
        this->users.insert({_users[i]->getName(), _users[i]});
    }
    this->users_mutex.unlock();
}

error_t Server::listen()
{
    this->running = true;
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
            Session* clientSession = new Session(clientSocketPtr);
            Authenticator auth = Authenticator(clientSocketPtr);
            auth_t authResponse = auth.authenticate();
            if(authResponse.isValid)
            {
                this->users_mutex.lock();
                std::cout << "DEBUG: Checking for user" << std::endl;
                User* user;
                if (this->users.find(authResponse.username) != this->users.end())
                {
                    user = this->users.at(authResponse.username);
                    if (user->countSessions() >= 2)
                    {
                        std::cout << "INFO: User `" << authResponse.username << "` has exceed its sessions limit" << std::endl;
                        std::cout << "INFO: Connection refused for user " << authResponse.username << std::endl;
                        clientSession->close(ERROR_NOTIFICATION_UNAUTHORIZED);
                        delete clientSession;
                    }
                    else
                    {
                        std::cout << "DEBUG: Adding session for `" << authResponse.username << "`" << std::endl;
                        user->addSession(clientSession);
                        std::cout << "INFO: Connection accepted for user " << authResponse.username << std::endl;
                        clientSession->listen();
                    }
                }
                else
                {
                    std::cout << "DEBUG: User `" << authResponse.username << "` not found" << std::endl;
                    user = new User(authResponse.username);

                    std::cout << "DEBUG: Adding session for `" << authResponse.username << "`" << std::endl;
                    user->addSession(clientSession);
    
                    this->users.insert({user->getName(), user});
                    std::cout << "INFO: Connection accepted for user " << authResponse.username << std::endl;
                    clientSession->listen();
                }
                this->users_mutex.unlock();
            }
            else
            {
                delete clientSession;
            }
        }
    }

    return 0;
}
