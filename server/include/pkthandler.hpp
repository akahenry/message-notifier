#ifndef PKTHANDLER_HPP
#define PKTHANDLER_HPP

#include <map>
#include <thread>

#include "user.hpp"
#include "queue.hpp"

class PacketHandler
{
    private:
        std::map<std::string, User*> users;
        std::mutex users_mutex;
        Queue* queue;
        std::thread thread;
        bool running;

        void run();
        void handleMessage(uint16_t session_id, packet pkt);
        void handleFollow(uint16_t session_id, packet pkt);
        void handleExit(uint16_t session_id, packet pkt);
    public:
        PacketHandler();
        PacketHandler(Queue* queue);
        
        void handle();
        void close();
        void addUser(User* user);
        bool hasUser(User* user);
        bool hasUser(std::string username);
        User* getUser(std::string username);
};

#endif