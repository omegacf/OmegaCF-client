#ifndef BMC_HPP
#define BMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include <limits>

class BestMoveCalculator {
    private:
        Game _game;
    public:
        BestMoveCalculator(Game game) : _game(game) {};
        int evaluateBoard(Grid grid, Player player, Player opponent);
        // checkline(3)
        // 1101, 1011, ...
        bool checkLine(int numberOfStones, bool gravity, Grid grid, Player player);
};
#endif