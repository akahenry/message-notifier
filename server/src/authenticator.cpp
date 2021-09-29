#include "authenticator.hpp"

Authenticator::Authenticator(Socket _socket)
{
    this->socket = _socket;
}

auth_t Authenticator::authenticate()
{
    packet pkt;
    auth_t response = auth_t{"", false};

    std::cout << "DEBUG: Running authenticator thread" << std::endl;

    while(true)
    {
        if (this->socket.receive(&pkt))
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