#include "client.hpp"

Client::Client(std::string _user, server_attr _server) 
{
    *this = Client(_user, _server.address, server.port);
}

Client::Client(std::string _user, std::string _address, std::string _port) : username{_user}
{
    this->server.address = _address;
    this->server.port = _port;
    this->socket = Socket(_address, _port);
}

error_t Client::connect()
{
    if (this->socket.configure(SOCKET_TYPE_CLIENT))
    {
        return ERROR_CONNECTION_SOCKET_FAILED;
    }

    if (this->socket.send(PACKET_TYPE_CONNECT, this->username, ""))
    {
        return ERROR_CONNECTION_SOCKET_FAILED;
    }

    this->listener = Listener(this->socket);

    return this->listener.start();
}

error_t Client::send(packet_type_t type, std::string message)
{
    return this->socket.send(type, this->username, message);
}

error_t Client::close()
{
    this->listener.stop();
    return this->socket.finish();
}