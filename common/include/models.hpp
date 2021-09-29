#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>

#define PACKET_PAYLOAD_MAX_LENGTH 512
#define PACKET_USERNAME_MAX_LENGTH 512

typedef enum __message_type_t
{
    MESSAGE_TYPE_PACKET,
    MESSAGE_TYPE_NOTIFICATION,
} message_type_t;

typedef enum __packet_type_t
{
    PACKET_TYPE_CMD,
    PACKET_TYPE_DATA,
    PACKET_TYPE_CONNECT,
    PACKET_TYPE_CLOSE,
} packet_type_t;

typedef struct __packet{ 
    packet_type_t type;        //Tipo do pacote (p.ex. DATA | CMD) 
    uint16_t seqn;        //Número de sequência 
    uint16_t length;       //Comprimento do payload  
    uint16_t timestamp;     //Timestamp do dado
    char _username[PACKET_USERNAME_MAX_LENGTH]; //Nome do usuário
    char _payload[PACKET_PAYLOAD_MAX_LENGTH];    //Dados da mensagem 
} packet;

typedef struct __server_attr
{
    std::string address;
    std::string port;
} server_attr;

typedef enum __error_notification_t
{
    ERROR_NOTIFICATION_NONE,
    ERROR_NOTIFICATION_UNAUTHORIZED,
    ERROR_NOTIFICATION_SERVER_CLOSED,
} error_notification_t;

typedef struct __notification{ 
    uint32_t id;         //Identificador da notificação (sugere-se um identificador único) 
    uint32_t timestamp;     //Timestamp da notificação 
    uint16_t length;       //Tamanho da mensagem 
    uint16_t pending;      //Quantidade de leitores pendentes 
    std::string _string;     //Mensagem 
    std::string _user;      //Nome do usuário que enviou a mensagem
    error_notification_t error;
} notification; 

#endif