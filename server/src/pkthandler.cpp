#include "pkthandler.hpp"

PacketHandler::PacketHandler(Queue<packet_item>* queue) : queue(queue) {}

void PacketHandler::handle()
{
    this->running = true;
    this->thread = std::thread(&PacketHandler::run, this);
}

void PacketHandler::close()
{  
    // Notify close
    log(LOG_TYPE_DEBUG,  "Trying to lock users_mutex");
    this->users_mutex.lock();
    for (auto it = this->users.begin(); it != this->users.end(); it++)
    {
        std::vector<Session*> sessions = it->second->getSessions();
        for (size_t i = 0; i < sessions.size(); i++)
        {
            notification notif;
            notif.error = ERROR_NOTIFICATION_SERVER_WILL_CLOSE;
            sessions[i]->send(MESSAGE_TYPE_NOTIFICATION, (void*)&notif);
        }
    }
    this->users_mutex.unlock();
    log(LOG_TYPE_DEBUG,  "Unlocking users_mutex");
    
    // Stop pushing messages into the queue
    log(LOG_TYPE_DEBUG,  "Trying to lock queue");
    this->queue->lock();
    log(LOG_TYPE_DEBUG,  "Checking if the queue is empty");
    while(!this->queue->empty());
    log(LOG_TYPE_DEBUG,  "Queue is empty");
    this->running = false;
    log(LOG_TYPE_DEBUG,  "Setting running to false");
    log(LOG_TYPE_DEBUG,  "Trying to join thread");
    this->thread.join();
    log(LOG_TYPE_DEBUG,  "After join thread");

    // Close sessions
    log(LOG_TYPE_DEBUG,  "Trying to lock users_mutex");
    this->users_mutex.lock();
    for (auto it = this->users.begin(); it != this->users.end(); it++)
    {
        std::vector<Session*> sessions = it->second->getSessions();
        for (size_t i = 0; i < sessions.size(); i++)
        {
            sessions[i]->close(ERROR_NOTIFICATION_SERVER_CLOSED);
        }
    }
    this->users_mutex.unlock();
    log(LOG_TYPE_DEBUG,  "Unlocking users_mutex");
}

void PacketHandler::run()
{
    while (this->running)
    {
        if (!this->queue->empty())
        {
            packet_item item = this->queue->pop();
            packet pkt = item.pkt;

            if (pkt.type == PACKET_TYPE_CMD_SEND)
            {
                log(LOG_TYPE_INFO,  "Data `" + std::string(pkt._payload) + "` received from user `" +  std::string(pkt._username) + "`");
                this->handleMessage(pkt);
            }
            else if (pkt.type == PACKET_TYPE_CMD_FOLLOW)
            {
                log(LOG_TYPE_INFO,  "Follow `" + std::string(pkt._payload) + "` received from user `" + std::string(pkt._username) + "`");
                this->handleFollow(pkt);
            }
            else if (pkt.type == PACKET_TYPE_CMD_EXIT)
            {
                log(LOG_TYPE_INFO,  "Exit received from user `" + std::string(pkt._username) + "`");
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
        notification notif;
        notif.error = ERROR_NOTIFICATION_NONE;
        strcpy(notif._user, pkt._username);
        strcpy(notif._string, pkt._payload);
        notif.length = pkt.length;
        notif.timestamp = pkt.timestamp;

        followers[i]->pushNotification(notif);
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

void PacketHandler::save(std::string path)
{
    
    this->users_mutex.lock();
    for (auto it = this->users.begin(); it != this->users.end(); it++)
    {
        std::fstream file;
        std::string _path = path;
        file.open(_path.append(it->first), std::ios::binary | std::ios::out);
        if(file)
        {
            log(LOG_TYPE_DEBUG, "Saving user `" + it->first + "`"); 
            std::vector<std::string> followers;
            std::vector<notification> notifications = it->second->getPending();
            for (auto user : it->second->getFollowers())
            {
                followers.push_back(user->getName());
            }
            
            size_t followers_size = followers.size();
            file.write((char*)&followers_size, sizeof(size_t));
            for (size_t i = 0; i < followers_size; i++)
            {
                log(LOG_TYPE_DEBUG,  "Saving follower `" + followers[i] + "` for user `" + it->first + "`"); 
                size_t name_size = followers[i].size();
                file.write((char*)&name_size, sizeof(size_t));
                file.write((char*)followers[i].c_str(), name_size);
            }

            size_t notifications_size = notifications.size();
            file.write((char*)&notifications_size, sizeof(size_t));
            for (size_t i = 0; i < notifications_size; i++)
            {
                file.write((char*)&notifications[i].id, sizeof(uint32_t));
                file.write((char*)&notifications[i].timestamp, sizeof(int64_t));
                file.write((char*)&notifications[i].length, sizeof(uint16_t));
                file.write((char*)&notifications[i]._string, notifications[i].length);
                file.write((char*)&notifications[i]._user, PACKET_USERNAME_MAX_LENGTH);
                file.write((char*)&notifications[i].error, sizeof(error_notification_t));
            }
        }
        file.close();
    }
    
}