#ifndef RMC_HPP
#define RMC_HPP

#include "Game.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "PossibleMove.hpp"
#include "IBestMoveCalculator.hpp"
#include <vector>

class RandomMoveCalculator: public IBestMoveCalculator {
    private:
        Game _game;
        Player _player;
    public:
        RandomMoveCalculator(Game game, Player player) : _game(game), _player(player) {
            // generate seed
            srand (time(NULL));
        };
        RandomMoveCalculator() {};

        virtual PossibleMove getBestMove(Grid& grid);
};
#endif