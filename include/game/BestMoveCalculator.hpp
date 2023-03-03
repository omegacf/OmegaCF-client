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
        Game _game;
        Player _player;
        Player& getOpponent(Player& player);
        int _max = 100000;
        int _min = -100000;
        int _depth = 6;
    public:
        BestMoveCalculator(Game game, Player player, int depth) : _game(game), _player(player), _depth(depth) {
            // generate seed
            srand (time(NULL));
        };
        BestMoveCalculator() {};
        int evaluateBoard(Grid grid, Player player, Player opponent);

        virtual PossibleMove getBestMove(Grid& grid);
        static PossibleMove minimax(Grid& grid, int depth, int alpha = -100000, int beta = 100000, bool isMax = true);
};
#endif