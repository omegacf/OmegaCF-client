#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "networkAgent.hpp"
#include "../game/IBestMoveCalculator.hpp"
#include "qLearningAgent.hpp"
#include "../game/Game.hpp"
#include "../game/Player.hpp"
#include "../factory/GameFactory.hpp"
#include "../util/Debug.hpp"
#include "../util/enums.hpp"
#include "network.hpp"

#include "../game/BestMoveCalculator.hpp"
#include "../game/RandomMoveCalculator.hpp"
#include "../game/QLearningMoveCalculator.hpp"


class Trainer {
    private:
        NetworkAgent* _networkAgent;
        QLearningAgent* _agent;
        Network _net;
        Game _game;
        int _playerNumber = 1;
        IBestMoveCalculator* _bmc;

    public:
        Trainer(MoveCalculator mc, NetworkAgent& networkAgent);
        void train(int amountGames);
        ~Trainer();
};
#endif