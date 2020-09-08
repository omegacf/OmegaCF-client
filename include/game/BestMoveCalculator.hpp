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
    public:
        BestMoveCalculator(Game game, Player player) : _game(game), _player(player) {};
        BestMoveCalculator() {};
        int evaluateBoard(Grid grid, Player player, Player opponent);
        // checkline(3)
        // 1101, 1011, ...
        bool checkLine(int numberOfStones, bool gravity, Grid grid, Player player);

        PossibleMove minimax(Grid grid, int depth, bool isMax = true);
};
#endif