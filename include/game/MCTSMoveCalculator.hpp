#ifndef MCTSMC_HPP
#define MCTSMC_HPP

#include "IBestMoveCalculator.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "MCTSNode.h"
#include <cmath>
#include <queue>
#include <limits>
#include "../util/Debug.hpp"

#define WIN_SCORE 10
#define LOOSE_SCORE 0
#define DRAW_SCORE 0

class MCTSMoveCalculator: public IBestMoveCalculator {
    private:
        Game* _game;
        Player _player;

        MCTSNode* _selection(MCTSNode& node);
        MCTSNode* _expansion(MCTSNode& node);
        float _simulation(MCTSNode& node);
        void _backpropagation(MCTSNode& node, float value);
        void _clearTree(MCTSNode& root);

        float _calcUCB(MCTSNode& node);
    
    public:
        MCTSMoveCalculator() {};
        MCTSMoveCalculator(Player player, Game* game): _player(player), _game(game) {};
        
        virtual PossibleMove getBestMove(Grid& grid);
        unsigned int iterations = 1000;
};
#endif
