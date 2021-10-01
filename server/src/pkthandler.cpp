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
    std::cout << "DEBUG: Trying to lock users_mutex" << std::endl;
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
    std::cout << "DEBUG: Unlocking users_mutex" << std::endl;
    
    // Stop pushing messages into the queue
    std::cout << "DEBUG: Trying to lock queue" << std::endl;
    this->queue->lock();
    std::cout << "DEBUG: Checking if the queue is empty" << std::endl;
    while(!this->queue->empty());
    std::cout << "DEBUG: Queue is empty" << std::endl;
    this->running = false;
    std::cout << "DEBUG: Setting running to false" << std::endl;
    std::cout << "DEBUG: Trying to join thread" << std::endl;
    this->thread.join();
    std::cout << "DEBUG: After join thread" << std::endl;

    // Close sessions
    std::cout << "DEBUG: Trying to lock users_mutex" << std::endl;
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
    std::cout << "DEBUG: Unlocking users_mutex" << std::endl;
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
            std::cout << "DEBUG:: Saving user `" << it->first << "`" << std::endl; 
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
                std::cout << "DEBUG: Saving follower `" << followers[i] << "` for user `" << it->first << "`" << std::endl; 
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