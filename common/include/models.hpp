#include <string>

#define PACKET_PAYLOAD_MAX_LENGTH 512

typedef uint16_t message_type_t;
#define MESSAGE_TYPE_PACKET 0
#define MESSAGE_TYPE_NOTIFICATION 1

typedef uint16_t packet_type_t;
#define PACKET_TYPE_DATA 0
#define PACKET_TYPE_CMD 1
#define PACKET_TYPE_CONNECT 2

typedef struct __packet{ 
    packet_type_t type;        //Tipo do pacote (p.ex. DATA | CMD) 
    uint16_t seqn;        //Número de sequência 
    uint16_t length;       //Comprimento do payload  
    uint16_t timestamp;     // Timestamp do dado 
    char _payload[PACKET_PAYLOAD_MAX_LENGTH];    //Dados da mensagem 
} packet;

typedef struct __server_attr
{
    std::string address;
    std::string port;
} server_attr;

typedef struct __notification{ 
    uint32_t id;         //Identificador da notificação (sugere-se um identificador único) 
    uint32_t timestamp;     //Timestamp da notificação 
    uint16_t length;       //Tamanho da mensagem 
    uint16_t pending;      //Quantidade de leitores pendentes 
    std::string _string;     //Mensagem 
    std::string _user;      //Nome do usuário que enviou a mensagem
} notification; 