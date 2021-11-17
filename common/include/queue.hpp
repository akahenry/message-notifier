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
        bool locked;

    public:
        Queue() = default;
        Queue<T>(const Queue<T> &q)
        {
            this->locked = false;
            this->queue = q.queue;
        }

        void push(T item)
        {
            if (!this->locked)
            {
                this->mutex.lock();
                this->queue.push_back(item);
                this->mutex.unlock();
            }
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

        void lock()
        {
            this->locked = true;
        }

        void unlock()
        {
            this->locked = false;
        }

        std::vector<T> data()
        {
            return this->queue;
        }

        void operator=(const Queue &q) 
        { 
            this->queue = q.queue;
            this->locked = q.locked;
        }
};

#endif