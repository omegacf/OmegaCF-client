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
        const static short sizeSize = 1;

        unsigned int getBufferSize();
        std::string getNetworkMessage();
        std::string getMessage();
        unsigned int getLength();
        NetworkMessageType getMessageType();
        ~NetworkMessage();
};

class ClientNetworkMessage : public NetworkMessage{
    public:
        ClientNetworkMessage(int8_t x);
};

class ServerNetworkMessage : public NetworkMessage{
    private:
        struct GameConfigStruct
        {
            int8_t playerNumber;
        };
        struct MoveStruct
        {
            int8_t x;
            int8_t playerNumber;
        };
        struct EndGameStruct
        {
            int8_t playerNumber;
        };
    public:
        MoveStruct Move;
        GameConfigStruct GameConfig;
        EndGameStruct EndGame;
        ServerNetworkMessage(std::string serverMessage);
        ServerNetworkMessage(NetworkMessageType type, unsigned int size, std::string serverMessage);
};
#endif