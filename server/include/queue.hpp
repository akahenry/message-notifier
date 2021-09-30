#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <vector>

#include "models.hpp"

template <typename T>
class Queue
{
    private:
        std::mutex mutex;
        std::vector<T> queue;

    public:
        void push(T item)
        {
            this->mutex.lock();
            this->queue.push_back(item);
            this->mutex.unlock();
        }

        T pop()
        {
            T item;

            this->mutex.lock();
            item = this->queue.front();
            this->queue.erase(this->queue.begin());
            this->mutex.unlock();

            return item;
        }

        bool empty()
        {
            this->mutex.lock();
            bool response = this->queue.size() == 0;
            this->mutex.unlock();
            return response;
        }
};

#endif