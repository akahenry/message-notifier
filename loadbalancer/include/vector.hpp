#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <mutex>

template <typename T>
class Vector
{
    using iterator=T*;
    private:
        std::vector<T> data;
        std::mutex mutex;

    public:
        T& at(size_t index)
        {
            this->mutex.lock();
            T& response = data[index];
            this->mutex.unlock();

            return response;
        }

        T& operator[](size_t index)
        {
            this->mutex.lock();
            T& response = data[index];
            this->mutex.unlock();

            return response;
        }

        void erase(size_t index)
        {
            this->mutex.lock();
            data.erase(data.begin() + index);
            this->mutex.unlock();
        }

        void push_back (const T& val)
        {
            this->mutex.lock();
            data.push_back(val);
            this->mutex.unlock();
        }

        int size()
        {
            int response;

            this->mutex.lock();
            response = data.size();
            this->mutex.unlock();

            return response;
        }

        iterator begin()
        {
            iterator response;

            this->mutex.lock();
            response = data.data();
            this->mutex.unlock();

            return response;
        }

        iterator end()
        {
            iterator response;

            this->mutex.lock();
            response = data.data() + data.size(); 
            this->mutex.unlock();

            return response;
        }
};

#endif