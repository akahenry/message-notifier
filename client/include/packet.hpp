#include <iostream>
#include <string>

#define PAYLOAD_MAX_SIZE 256

typedef uint16_t packet_type_t;
#define PACKET_TYPE_DATA 0
#define PACKET_TYPE_CMD 1

typedef struct __packet{ 
    packet_type_t type;        //Tipo do pacote (p.ex. DATA | CMD) 
    uint16_t seqn;        //Número de sequência 
    uint16_t length;       //Comprimento do payload  
    uint16_t timestamp;     // Timestamp do dado 
    char _payload[PAYLOAD_MAX_SIZE];    //Dados da mensagem 
} packet;