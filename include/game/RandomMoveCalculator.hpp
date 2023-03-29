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
        Player _player;
    public:
        RandomMoveCalculator(Player player) : _player(player) {};
        RandomMoveCalculator() {};

        virtual PossibleMove getBestMove(Grid& grid);
};
#endif