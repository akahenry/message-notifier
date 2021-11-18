#include "manager.hpp"
#include <string>

//testa se a string é um número
bool isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    //ex.:5432 localhost:5433 localhost:5444
    std::vector<server_attr> vec;
    std::string port;
    if (isNumber(argv[1]) == true)
    {
        port = argv[1];
        std::string address;
        std::string port;
        for (int i = 2; i < argc; i++)
        {
            std::string s = argv[i];
            //criar 2 tokens e atribuir ao endereço e à porta
            std::string delim = ":";
            auto start = 0;
            auto end = s.find(":");
            while (end != std::string::npos)
            {
                bool address_filled = false;
                std::string temp = s.substr(start, end - start);
                if (address_filled == false)
                {
                    address = temp;
                    address_filled == true;
                } else {
                    port = temp;
                }                
                start = end + delim.length();
                end = s.find(delim, start);
            }
            port = s.substr(start, end - start);
            vec.push_back(server_attr{address, port}); 
        }
    }
    
    Manager manager = Manager(port, vec);
    manager.listen();
    manager.serve();

    std::cout << "oi";

    return 0;
}