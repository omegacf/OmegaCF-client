#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <iostream>
#include <string>
#include "network/NetworkMessage.hpp"
#include "network/DataHandlingService.hpp"

class GameHandler
{
    private:
        short _playerNumber;
        bool handleMessage(ServerNetworkMessage message);
        void move(ServerNetworkMessage message);
        void handleDisqualification(ServerNetworkMessage message);
        void handleMoveRequest(ServerNetworkMessage message);
    public:
        GameHandler(std::string host, unsigned short port);
        void run();
        ~GameHandler();
};
#endif