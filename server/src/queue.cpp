#include "queue.hpp"

void Queue::push(packet_item item)
{
    this->mutex.lock();
    this->queue.push_back(item);
    this->mutex.unlock();
}
        
packet_item Queue::pop()
{
    packet_item item;

    this->mutex.lock();
    item = this->queue.front();
    this->queue.erase(this->queue.begin());
    this->mutex.unlock();

    return item;
}

bool Queue::empty()
{
    this->mutex.lock();
    bool response = this->queue.size() == 0;
    this->mutex.unlock();
    return response;
}