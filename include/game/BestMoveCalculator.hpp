#ifndef BMC_HPP
#define BMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "PossibleMove.hpp"
#include "IBestMoveCalculator.hpp"
#include <limits>
#include <algorithm>

class BestMoveCalculator: public IBestMoveCalculator {
    private:
        Game* _game;
        Player _player;
        Player& getOpponent(Player& player);
        int _max = std::numeric_limits<int>::max();
        int _min = std::numeric_limits<int>::min();
        int _depth = 6;
    public:
        BestMoveCalculator(Game* game, Player player, int depth) : _game(game), _player(player), _depth(depth) {};
        BestMoveCalculator() {};
        int evaluateBoard(Grid grid, Player player, Player opponent);

        virtual PossibleMove getBestMove(Grid& grid);
        PossibleMove minimax(Grid& grid, int depth, int alpha = -100000, int beta = 100000, bool isMax = true);
};
#endif