#ifndef BMC_HPP
#define BMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "PossibleMove.hpp"
#include <limits>
#include <algorithm>

class BestMoveCalculator {
    private:
        Game _game;
        Player _player;
        Player& getOpponent(Player& player);
        int _max = 100000;
        int _min = -100000;
    public:
        BestMoveCalculator(Game game, Player player) : _game(game), _player(player) {};
        BestMoveCalculator() {};
        int evaluateBoard(Grid grid, Player player, Player opponent);
        bool checkLine(int numberOfStones, Grid& grid, Player player);

        PossibleMove minimax(Grid& grid, int depth, int alpha = -100000, int beta = 100000, bool isMax = true);
};
#endif