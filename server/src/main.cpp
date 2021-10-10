#include <csignal>
#include <filesystem>
#include <fstream>

#include "server.hpp"

#define DATA_PATH "./server/data/"

std::vector<User*> users;
Server* server;

std::vector<User*> loadUsers(std::string path);
void saveUsers(int signum);

int main(int argc, char **argv)
{
    std::string port;

    if (argc == 2)
    {
        port = argv[1];
    } 
    else 
    {
        std::cout << "Enter the port number: ";
        std::cin >> port;
    }

    users = loadUsers(DATA_PATH);

    std::signal(SIGINT, saveUsers);

    server = new Server(users, port);

    if (server->listen())
    {
        std::cout << "Error configurating server. Exiting..." << std::endl;
        return ERROR_CONFIGURATION_FAILED;
    }

    server->serve();

    return 0;
}

std::vector<User*> loadUsers(std::string path)
{
    std::map<std::string, User*> map;
    std::vector<User*> response;

    for (const auto & entry : std::filesystem::directory_iterator(DATA_PATH))
    {
        std::fstream file;
        size_t vector_size, name_size;
        char username[PACKET_USERNAME_MAX_LENGTH] = {'\0'};
        User* user;
        std::vector<std::string> followers;
        std::vector<notification> notifications;

        strcpy(username, entry.path().filename().c_str());
        log(LOG_TYPE_DEBUG,  "Reading user `" + std::string(username) + "`"); 
        if (map.find(username) == map.end())
        {
            user = new User(username);
            map.insert({username, user});
        }
        else
        {
            user = map.at(username);
        }

        file.open(entry.path().string(), std::ios::binary | std::ios::in);

        file.read((char*)&vector_size, sizeof(size_t));
        for (size_t i = 0; i < vector_size; i++)
        {
            User* follower;
            char follower_name[PACKET_USERNAME_MAX_LENGTH] = {"\0"};
            file.read((char*)&name_size, sizeof(size_t));
            file.read((char*)follower_name, name_size);

            log(LOG_TYPE_DEBUG,  "Reading follower `" + std::string(follower_name) + "` for user `" + std::string(username) + "`"); 

            if (map.find(follower_name) == map.end())
            {
                follower = new User(follower_name);
                map.insert({follower_name, follower});
            }
            else
            {
                follower = map.at(follower_name);
            }
            follower->follow(user);
        }

        file.read((char*)&vector_size, sizeof(size_t));
        for (size_t i = 0; i < vector_size; i++)
        {
            notification notif;
            file.read((char*)&notif.id, sizeof(uint32_t));
            file.read((char*)&notif.timestamp, sizeof(int64_t));
            file.read((char*)&notif.length, sizeof(uint16_t));
            file.read((char*)&notif._string, notif.length);
            file.read((char*)&notif._user, PACKET_USERNAME_MAX_LENGTH);
            file.read((char*)&notif.error, sizeof(error_notification_t));

            user->pushNotification(notif);
        }

        response.push_back(user);
        
        file.close();
    }

    return response;
}

void saveUsers(int signum)
{
    log(LOG_TYPE_DEBUG, "Interrupt signal (" + std::to_string(signum) + ") received. Exiting...");

    server->close();
    
    server->save(DATA_PATH);

    exit(signum);  
}