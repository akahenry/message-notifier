#include "socket.hpp"

Socket::Socket(std::string _port)
{
    sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(std::stoi(_port));
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(sockaddr.sin_zero), 8);  
}

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

error_t Socket::configure(socket_type_t type)
{
    if (type == SOCKET_TYPE_CLIENT)
    {
        return this->configureClient();
    }
    else if (type == SOCKET_TYPE_SERVER)
    {
        return this->configureServer();
    }
    else
    {
        return ERROR_WRONG_SOCKET_TYPE;
    }
}

error_t Socket::configureClient()
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

error_t Socket::configureServer()
{
    struct sockaddr_in address = this->sockaddr;
    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        std::cout << "Error opening socket" << std::endl;
        return ERROR_OPEN_SOCKET;
    }
	
	if (bind(this->sockfd, (struct sockaddr *) &address, sizeof(address)) == -1)
    {
        return ERROR_CONFIGURATION_FAILED;
    }

    this->sockaddr = address;

    if (listen(this->sockfd, 5) == -1)
    {
        return ERROR_LISTEN_FAILED;
    }

    return 0;
}

error_t Socket::finish()
{
    return close(this->sockfd);
}

error_t Socket::send(packet_type_t type, std::string username, std::string message)
{
    const auto p1 = std::chrono::system_clock::now();
    packet pkt;
    pkt.type = type;
    pkt.length = PACKET_PAYLOAD_MAX_LENGTH;
    strcpy(pkt._payload, message.c_str());
    strcpy(pkt._username, username.c_str());
    pkt.timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

    return this->send(pkt);
}

error_t Socket::send(packet pkt)
{
    if (write(this->sockfd, &pkt, sizeof(pkt)) == -1)
    {
        return ERROR_SENDING_MESSAGE;
    }

    return 0;
}

error_t Socket::send(notification notif)
{
    if (write(this->sockfd, &notif, sizeof(notif)) == -1)
    {
        return ERROR_SENDING_MESSAGE;
    }

    return 0;
}

error_t Socket::receive(notification* _notification)
{
    int res = read(this->sockfd, _notification, sizeof(notification));
    if (res == -1)
    {
        return ERROR_RECEIVING_MESSAGE;
    }
    else if (res == 0)
    {
        return ERROR_BROKEN_SOCKET_CONNECTION;
    }

    return 0;
}

error_t Socket::receive(packet* pkt)
{
    int res = read(this->sockfd, pkt, sizeof(packet));
    if (res == -1)
    {
        return ERROR_RECEIVING_MESSAGE;
    }
    else if (res == 0)
    {
        return ERROR_BROKEN_SOCKET_CONNECTION;
    }

    return 0;
}

error_t Socket::serve(Socket *socket)
{
    struct sockaddr_in clientAddress;
    int clientSockfd;
    socklen_t clientLen = sizeof(clientAddress);

    clientSockfd = accept(this->sockfd, (struct sockaddr *) &clientAddress, &clientLen);

    if (clientSockfd == -1)
    {
        return ERROR_ACCEPTING_SOCKET_CONNECTION;
    }
    
    socket->sockaddr = clientAddress;
    socket->sockfd = clientSockfd;

    return 0;
}