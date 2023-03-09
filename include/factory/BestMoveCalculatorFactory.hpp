#ifndef BESTMOVECALCULATOR_HPP
#define BESTMOVECALCULATOR_HPP

#include "../game/BestMoveCalculator.hpp"
#include "../game/IBestMoveCalculator.hpp"
#include <string>

class GameFactory{
    public:
        static Game create(int sizeX, int sizeY, int playerNumber);
};
#endif