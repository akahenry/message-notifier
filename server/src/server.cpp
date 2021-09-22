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
            clientSession->listen();
        }
    }

    return 0;
}
