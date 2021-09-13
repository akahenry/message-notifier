#include <client.hpp>

Client::Client(User _user, Server _server) : user{_user}, server{_server} {}

Client::Client(User _user, std::string _hostname, std::string _port) : user{_user}
{
    this->server.hostname = _hostname;
    this->server.port = _port;
}

error_t Client::connect()
{
    std::cout << "Error connecting to `" << this->server.hostname << ":" << this->server.port << "`. Exiting...." << std::endl;
    return ERROR_CONNECTION_FAILED;
}