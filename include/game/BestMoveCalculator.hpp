#ifndef BMC_HPP
#define BMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "PossibleMove.hpp"
#include "IBestMoveCalculator.hpp"
#include <limits>
#include <algorithm>
#include "../util/enums.hpp"

#define MIN_VAL -100000
#define MAX_VAL 100000

class BestMoveCalculator: public IBestMoveCalculator {
    private:
        Game* _game;
        Player _player;
        Player& getOpponent(Player& player);
        int _max = MAX_VAL;
        int _min = MIN_VAL;
        int _depth = 6;
        int _countFreeStones(LineType type, std::pair<int, int>& begin, std::pair<int, int>& end, Grid& grid, Player& player, int numberOfStones);
        int _evaluateBoardForOnePlayer(Grid& grid, Player player, Player opponent);
    public:
        BestMoveCalculator(Game* game, Player player, int depth) : _game(game), _player(player), _depth(depth) {};
        BestMoveCalculator() {};
        int evaluateBoard(Grid& grid, Player player, Player opponent);

        virtual PossibleMove getBestMove(Grid& grid);
        PossibleMove minimax(Grid& grid, int depth, int alpha = MIN_VAL, int beta = MAX_VAL, bool isMax = true);
};
#endif