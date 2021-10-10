#include "logger.hpp"

log_type_t log_level = LOG_TYPE_INFO;

void log(log_type_t type, std::string message)
{
    switch (log_level)
    {
        case LOG_TYPE_INFO:
            switch (type)
            {
                case LOG_TYPE_INFO:
                    std::cout << "INFO: " << message << std::endl;
                    break;
                default:
                    break;
            }
            break;
        case LOG_TYPE_DEBUG:
            switch (type)
            {
                case LOG_TYPE_INFO:
                    std::cout << "INFO: " << message << std::endl;
                    break;
                case LOG_TYPE_DEBUG:
                    std::cout << "DEBUG: " << message << std::endl;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}