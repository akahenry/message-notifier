#include "listener.hpp"

Listener::Listener(Socket _socket) : socket{_socket} {}

error_t Listener::run()
{
    notification notif;

    while(this->running)
    {
        if (this->socket.receive(&notif))
        {
            this->running = false;
            return ERROR_RECEIVING_MESSAGE;
        }

        std::cout << "\n----------------------------------\n";
        std::cout << notif._user << ": " << notif._string;
        std::cout << "----------------------------------\n";

        std::cout << "> ";
        fflush(stdout);
    }

    return 0;
}

error_t Listener::start()
{
    this->running = true;
    this->thread = std::thread(&Listener::run, this);

    return 0;
}

error_t Listener::stop()
{
    this->running = false;
    this->thread.join();

    return 0;
}