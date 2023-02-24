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

class Agent {
    private:
        std::vector<std::string> _states; // state is represented as a hash
        float _expRate = 0;
        std::map<std::string, std::vector<double>> _Q;
        Player _player;
        Game _game;
        float _alpha = 0;
        float _gamma = 0;

        int getMaxAction(std::map<std::string, std::vector<double>>::iterator& state);
        void updateQ(std::string state, uint8_t action, std::string newState, double reward);

    public:
        Agent(Player& player, Game& game, float expRate=0.3, float alpha = 0.01, float gamma = 0.9) 
        : _expRate(expRate), _player(player), _game(game), _alpha(alpha), _gamma(gamma) {
            // generate seed
            srand (time(NULL));
        };
        PossibleMove chooseAction(Grid& grid);

};
#endif