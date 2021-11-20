#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <thread>
#include <functional>
#include <algorithm>

#include "socket.hpp"
#include "queue.hpp"

template <typename T>
class Publisher
{
    private:
        Queue<T>* queue;
        std::vector<Socket> sockets;
        std::thread thread;
        bool running;

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
                if (!this->queue->empty())
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

        void join()
        {
            this->thread.join();
        }

        void addSocket(Socket socket)
        {
            bool already = false;

            for (size_t i = 0; i < this->sockets.size(); i++)
            {
                if (this->sockets[i] == socket)
                {
                    already = true;
                    break;
                }
            }
            
            if (!already)
            {
                this->sockets.push_back(socket);
            }
        }

        void removeSocket(Socket socket)
        {
            for (size_t i = 0; i < this->sockets.size(); i++)
            {
                if (this->sockets[i] == socket)
                {
                    this->sockets[i].finish();
                    this->sockets.erase(this->sockets.begin() + i);
                }
            }
        }

        const std::vector<Socket> getSockets()
        {
            return this->sockets;
        }
};

#endif