#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "networkAgent.hpp"
#include "../game/IBestMoveCalculator.hpp"
#include "qLearningAgent.hpp"
#include "../game/Game.hpp"
#include "../factory/GameFactory.hpp"
#include "../util/Debug.hpp"


class Trainer {
    private:
        NetworkAgent* _networkAgent;
    public:
        Trainer() {};
        Trainer(NetworkAgent* nwAgent):_networkAgent(nwAgent){};
        void train(int amountGames, QLearningAgent& qAgent, IBestMoveCalculator* opponent);
};
#endif