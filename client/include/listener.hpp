#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <thread>
#include <functional>

#include "socket.hpp"

class Listener
{
    private:
        Socket socket;
        std::thread thread;
        bool running = 0;

        error_t run();

    public:
        Listener() = default;
        Listener(Socket _socket);
        
        error_t start();
        error_t stop();
};

#endif