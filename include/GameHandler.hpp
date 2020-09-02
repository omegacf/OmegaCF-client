#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <string>
#include "network/DataHandlingService.hpp"
#include "factory/GameFactory.hpp"

class GameHandler{
    private:
        short _playerNumber;
        Game _game;
        Player getPlayer(int8_t playerNumber);
    public:
        GameHandler(std::string host, unsigned short port);
        void handleDisqualification(ServerNetworkMessage sm);
        void handleMoveRequest(ServerNetworkMessage sm);
        void handleMove(ServerNetworkMessage sm);
        bool handleMessage(ServerNetworkMessage sm);
        void handeEndGame(ServerNetworkMessage sm);
        void run();
        ~GameHandler();
};
#endif