#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <thread>
#include <functional>

#include "socket.hpp"
#include "queue.hpp"

template <typename T>
class Publisher
{
    private:
        Queue<T>* queue;
        std::vector<Socket> sockets;
        std::thread thread;
        bool running = 0;

    public:
        Publisher() = default;
        Publisher<T>(std::vector<Socket> _sockets, Queue<T>* _queue) : sockets{_sockets}, queue{_queue} {}
        
        error_t start()
        {
            this->running = true;
            this->thread = std::thread(&Publisher::run, this);

            return 0;
        }

        error_t stop()
        {
            this->running = false;

            return 0;
        }

        error_t run()
        {
            while(this->running)
            {
                if (!this->queue->empty)
                {
                    T item = this->queue->pop();
                    for (size_t i = 0; i < this->sockets.size(); i++)
                    {
                        this->sockets[i].send(item);
                    }
                }
            }

            return 0;
        }

};

#endif