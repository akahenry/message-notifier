#include <string>
#include <vector>

class User
{
    private:
        std::string name;
        std::vector<User*> following;
        std::vector<User*> followers;

    public:
        User();
        User(std::string _name);
        User(std::string _name, std::vector<User*> _following, std::vector<User*> _followers);
        void addFollower(User* user);
        void follow(User* user);

        std::vector<User*> getFollowers();
        std::string getName();
};