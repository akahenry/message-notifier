#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <thread>
#include <functional>

#include "socket.hpp"
#include "queue.hpp"

template <typename T>
class Listener
{
    protected:
        Socket socket;
        Queue<T>* queue;
        bool (*analyze)(T, Listener<T>*);
        std::thread thread;
        bool running;

    public:
        Listener() = default;
        Listener<T>(Socket _socket, Queue<T>* _queue) : socket{_socket}, queue{_queue} {}
        Listener<T>(Socket _socket, Queue<T>* _queue, bool (*analyze)(T)) : socket{_socket}, queue{_queue}, analyze{analyze} {}
        
        error_t start()
        {
            this->running = true;
            this->thread = std::thread(&Listener::run, this);

            return 0;
        }

        error_t stop()
        {
            this->running = false;

            return 0;
        }

        error_t run()
        {
            T item;

            while(this->running)
            {
                if (this->socket.receive(&item))
                {
                    this->stop();
                    return ERROR_RECEIVING_MESSAGE;
                }

                if (this->analyze == NULL || this->analyze(item, this))
                {
                    this->queue->push(item);
                }
            }

            return 0;
        }

        void join()
        {
            this->thread.join();
        }
};

#endif