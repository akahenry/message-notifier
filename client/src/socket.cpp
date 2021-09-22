#include "socket.hpp"

Socket::Socket(std::string _address, std::string _port)
{
    struct hostent *host;
    host = gethostbyname(_address.c_str());

    if (host == NULL) 
    {
        std::cout << "Server address `" << _address << "` is not valid. Throwing an error..." << std::endl;
        throw std::invalid_argument("Server address must be a valid value");
    }
    
    this->sockaddr.sin_family = AF_INET;     
    this->sockaddr.sin_port = htons(std::stoi(_port));    
    this->sockaddr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(this->sockaddr.sin_zero), 8);
}

const int Socket::getSocket()
{
    return this->sockfd;
}

error_t Socket::configure()
{
    struct sockaddr_in address = this->sockaddr;
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        std::cout << "Error opening socket" << std::endl;
        return ERROR_OPEN_SOCKET;
    }
	
	if (connect(this->sockfd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        return ERROR_CONFIGURATION_FAILED;
    }

    this->sockaddr = address;

    return 0;
}

error_t Socket::finish()
{
    return close(this->sockfd);
}

error_t Socket::send(packet_type_t type, std::string message)
{
    packet pkt;
    pkt.type = type;
    pkt.length = message.length();
    pkt._payload = message;
    pkt.timestamp = 0;

    if (write(this->sockfd, &pkt, sizeof(pkt)) == -1)
    {
        return ERROR_SENDING_MESSAGE;
    }

    return 0;
}

error_t Socket::receive(notification* _notification)
{
    if (read(this->sockfd, &_notification, sizeof(notification)) == -1)
    {
        return ERROR_RECEIVING_MESSAGE;
    }

    return 0;
}