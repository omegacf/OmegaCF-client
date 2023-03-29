#ifndef AGENT_HPP
#define AGENT_HPP

#include <vector>
#include <map>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "../game/Grid.hpp"
#include "../game/Game.hpp"
#include "../game/PossibleMove.hpp"
#include "../game/Player.hpp"

#include "networkAgent.hpp"

class QLearningAgent {
    private:
        // std::vector<std::string> _states; // state is represented as a hash
        float _expRate = 0;
        // std::map<std::string, std::vector<double>> _Q;
        Player _player;
        float _alpha = 0;
        float _gamma = 0;
        NetworkAgent* _networkAgent;

        void updateQ(std::string state, uint8_t action, std::string newState, double reward);
        PossibleMove getActionFromQValues(std::vector<PossibleMove>& possibleMoves, std::array<float, 7>& qValues);

    public:
        QLearningAgent(Player player, NetworkAgent* nwAgent, float expRate=0.3, float alpha = 0.01, float gamma = 0.9) 
        : _expRate(expRate), _player(player), _networkAgent(nwAgent), _alpha(alpha), _gamma(gamma) {};
        QLearningAgent(){};
        PossibleMove chooseAction(Grid& grid);

};
#endif