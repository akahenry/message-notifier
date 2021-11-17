#include "client.hpp"

int main(int argc, char **argv)
{
    Socket client_socket = Socket("localhost", "1234");
    Socket server_1 = Socket("localhost", "3001");
    Socket server_2 = Socket("localhost", "3002");
    client_socket.configure(SOCKET_TYPE_CLIENT);
    server_1.configure(SOCKET_TYPE_SERVER);
    server_2.configure(SOCKET_TYPE_SERVER);
    std::vector<Socket> server_sockets = {server_1, server_2};
    Client client = Client(client_socket, server_sockets);

    return 0;
}