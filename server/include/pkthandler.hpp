#ifndef PKTHANDLER_HPP
#define PKTHANDLER_HPP

#include <map>
#include <thread>
#include <fstream>

#include "user.hpp"
#include "queue.hpp"

class PacketHandler
{
    private:
        std::map<std::string, User*> users;
        std::mutex users_mutex;
        Queue<packet_item>* queue;
        std::thread thread;
        bool running;

        void run();
        void handleMessage(packet pkt);
        void handleFollow(packet pkt);
        void handleExit(uint16_t session_id, packet pkt);
    public:
        PacketHandler();
        PacketHandler(Queue<packet_item>* queue);
        
        void handle();
        void close();
        void addUser(User* user);
        bool hasUser(User* user);
        bool hasUser(std::string username);
        User* getUser(std::string username);
        void save(std::string path);
};

#endif