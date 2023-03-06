#ifndef QMC_HPP
#define QMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "PossibleMove.hpp"
#include "IBestMoveCalculator.hpp"
#include "../ml/qLearningAgent.hpp"
#include "../ml/network.hpp"
#include "../ml/networkAgent.hpp"
#include "../util/enums.hpp"

class QLearningMoveCalculator: public IBestMoveCalculator {
    private:
        Player _player;
        QLearningAgent* _agent;
        Network net;
        NetworkAgent* _netAgent;
    public:
        QLearningMoveCalculator(Player player);
        QLearningMoveCalculator() {};

        virtual PossibleMove getBestMove(Grid& grid);
        ~QLearningMoveCalculator();
};
#endif