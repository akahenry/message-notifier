#ifndef SERVER_HPP
#define SERVER_HPP

#include "user.hpp"
#include "client.hpp"
#include "vector.hpp"

class Server : public User<notification>
{
    protected:
        void onStopListener();
        void onStopPublisher();
    
    public:
        typedef struct _on_stop_t
        {
            void (*function)(Socket, Vector<Server*>*, Vector<Client*>*);
            Vector<Server*>* servers;
            Vector<Client*>* clients;
        } on_stop_t;

        Server(Socket listen_socket, on_stop_t on_stop);
        Server(Socket listen_socket, std::vector<Socket> publish_sockets, on_stop_t on_stop);

    private:
        on_stop_t on_stop;

        static bool isNotServerCloseNotification(notification notif);
};

#endif