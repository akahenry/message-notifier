#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <vector>

#include "models.hpp"

class Queue
{
    private:
        std::mutex mutex;
        std::vector<packet_item> queue;

    public:
        void push(packet_item item);
        packet_item pop();
        bool empty();
};

#endif