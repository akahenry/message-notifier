#include "pkthandler.hpp"

PacketHandler::PacketHandler(Queue<packet_item>* queue) : queue(queue) {}

void PacketHandler::handle()
{
    this->running = true;
    this->thread = std::thread(&PacketHandler::run, this);
}

void PacketHandler::close()
{
    this->running = false;
}

void PacketHandler::run()
{
    while(this->running)
    {
        if (!this->queue->empty())
        {
            packet_item item = this->queue->pop();
            packet pkt = item.pkt;

            if (pkt.type == PACKET_TYPE_CMD_SEND)
            {
                std::cout << "INFO: Data `" << pkt._payload << "` received from user `" <<  pkt._username << "`" << std::endl;
                this->handleMessage(pkt);
            }
            else if (pkt.type == PACKET_TYPE_CMD_FOLLOW)
            {
                std::cout << "INFO: Follow `" << pkt._payload << "` received from user `" <<  pkt._username << "`" << std::endl;
                this->handleFollow(pkt);
            }
            else if (pkt.type == PACKET_TYPE_CMD_EXIT)
            {
                std::cout << "INFO: Exit received from user `" <<  pkt._username << "`" << std::endl;
                this->handleExit(item.session_id, pkt);
            }
        }
    }
}

void PacketHandler::handleMessage(packet pkt)
{
    User* user = this->getUser(pkt._username);
    std::vector<User*> followers = user->getFollowers();
    for (size_t i = 0; i < followers.size(); i++)
    {
        std::vector<Session*> sessions = followers[i]->getSessions();
        for (size_t j = 0; j < sessions.size(); j++)
        {
            notification notif;
            notif.error = ERROR_NOTIFICATION_NONE;
            strcpy(notif._user, pkt._username);
            strcpy(notif._string, pkt._payload);
            notif.length = pkt.length;
            sessions[j]->send(MESSAGE_TYPE_NOTIFICATION, &notif);
        }
    }
}

void PacketHandler::handleFollow(packet pkt)
{
    User* follower = this->getUser(pkt._username);
    User* followed = this->getUser(pkt._payload);

    if(follower != NULL && followed != NULL)
    {
        follower->follow(followed);
    }
}

void PacketHandler::handleExit(uint16_t session_id, packet pkt)
{
    User* user = this->getUser(pkt._username);
    user->removeSession(session_id);
}

bool PacketHandler::hasUser(User* user)
{
    this->users_mutex.lock();
    bool response = this->users.find(user->getName()) != this->users.end();
    this->users_mutex.unlock();
    return response;
}

bool PacketHandler::hasUser(std::string username)
{
    User user{username};
    return this->hasUser(&user);
}

void PacketHandler::addUser(User* user)
{
    if (!this->hasUser(user))
    {
        this->users_mutex.lock();
        this->users.insert({user->getName(), user});
        this->users_mutex.unlock();
    }
}

User* PacketHandler::getUser(std::string username)
{
    if (this->hasUser(username))
    {
        User* user;
        this->users_mutex.lock();
        user = this->users.at(username);
        this->users_mutex.unlock();

        return user;
    }
    else
    {
        return NULL;
    }
}