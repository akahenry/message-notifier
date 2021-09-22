#include <string>

typedef uint16_t packet_type_t;
#define PACKET_TYPE_DATA 0
#define PACKET_TYPE_CMD 1

typedef struct __packet{ 
    packet_type_t type;        //Tipo do pacote (p.ex. DATA | CMD) 
    uint16_t seqn;        //Número de sequência 
    uint16_t length;       //Comprimento do payload  
    uint16_t timestamp;     // Timestamp do dado 
    std::string _payload;    //Dados da mensagem 
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