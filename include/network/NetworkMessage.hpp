#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

#include "NetworkMessageType.hpp"
#include "../util/NetworkMessageConverter.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class NetworkMessage{
    protected:
        NetworkMessageType _type;
        int _size;
        std::string _message;
    public:
        const static short typeSize = 1;
        const static short sizeSize = 4;

        unsigned int getBufferSize();
        std::string getNetworkMessage();
        std::string getMessage();
        unsigned int getLength();
        NetworkMessageType getMessageType();
        ~NetworkMessage();
};

class ClientNetworkMessage : public NetworkMessage{
    public:
        ClientNetworkMessage(NetworkMessageType type, int8_t number);
        ClientNetworkMessage(int16_t x, int16_t y, int8_t specialField);
};

class ServerNetworkMessage : public NetworkMessage{
    private:
        struct MoveStruct
        {
            int16_t x, y;
            int8_t specialField, playerNumber;
        };
    public:
        MoveStruct Move;
        ServerNetworkMessage(std::string serverMessage);
        ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage);
};
#endif