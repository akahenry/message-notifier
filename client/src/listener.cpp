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

        switch (notif.error)
        {
            case ERROR_NOTIFICATION_NONE:
                std::cout << "\n----------------------------------\n";
                std::cout << notif.timestamp << " " << notif._user << ": " << notif._string << std::endl;
                std::cout << "----------------------------------\n";

                std::cout << "> ";
                fflush(stdout);
                break;
            case ERROR_NOTIFICATION_UNAUTHORIZED:
                log(LOG_TYPE_INFO, "User has exceed limit of concurrent sessions");
                exit(2);
                break;
            
            case ERROR_NOTIFICATION_SERVER_CLOSED:
                log(LOG_TYPE_INFO, "Server has closed. Closing client...");
                exit(1);
                break;
        }
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

    return 0;
}