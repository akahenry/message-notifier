#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

typedef enum __log_type_t 
{
    LOG_TYPE_INFO,
    LOG_TYPE_DEBUG
} log_type_t;

void log(log_type_t type, std::string message);

#endif