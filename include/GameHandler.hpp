#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <string>
#include "network/DataHandlingService.hpp"
#include "factory/GameFactory.hpp"
#include "game/BestMoveCalculator.hpp"
#include "game/RandomMoveCalculator.hpp"
#include "game/QLearningMoveCalculator.hpp"
#include <limits>
#include "util/enums.hpp"

class GameHandler{
    private:
        short _playerNumber;
        Game _game;
        IBestMoveCalculator* _bmc;
    public:
        GameHandler(std::string host, unsigned short port);
        void handleDisqualification(ServerNetworkMessage sm);
        void handleMoveRequest(ServerNetworkMessage sm);
        void handleMove(ServerNetworkMessage sm);
        bool handleMessage(ServerNetworkMessage sm);
        void handleEndGame(ServerNetworkMessage sm);
        void run(MoveCalculator mc);
        ~GameHandler();
};
#endif