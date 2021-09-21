#include "client.hpp"

Client::Client(std::string _user, server_attr _server) 
{
    *this = Client(_user, _server.address, server.port);
}

Client::Client(std::string _user, std::string _address, std::string _port) : username{_user}
{
    struct hostent *host;
    host = gethostbyname(_address.c_str());

    if (host == NULL) 
    {
        std::cout << "Server address `" << _address << "` is not valid. Throwing an error..." << std::endl;
        throw std::invalid_argument("Server address must be a valid value");
    }
    else
    {
        this->server.address = _address;
        this->server.port = _port;

        this->sockaddr.sin_family = AF_INET;     
	    this->sockaddr.sin_port = htons(std::stoi(_port));    
	    this->sockaddr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(this->sockaddr.sin_zero), 8);
    }
}

error_t Client::configure()
{
    struct sockaddr_in address = this->sockaddr;
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        std::cout << "Error opening socket" << std::endl;
        return ERROR_OPEN_SOCKET;
    }
	
	if (connect(this->sockfd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        std::cout << "Error connecting to `" << this->server.address << ":" << this->server.port << "`" << std::endl;
        return ERROR_CONFIGURATION_FAILED;
    }

    this->sockaddr = address;

    return 0;
}

error_t Client::sendMessage(packet_type_t type, std::string message)
{
    packet pkt;
    pkt.type = type;
    pkt.length = message.length();
    strcpy(pkt._payload, message.c_str());
    pkt.timestamp = 0;

    if (write(sockfd, &pkt, sizeof(pkt)) == -1)
    {
        return ERROR_SENDING_MESSAGE;
    }

    return 0;
}

error_t Client::finish()
{
    return close(this->sockfd);
}