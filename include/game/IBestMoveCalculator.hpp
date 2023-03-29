#ifndef IBESTMOVECALCULATOR_HPP
#define IBESTMOVECALCULATOR_HPP

#include "Grid.hpp"
#include "PossibleMove.hpp"

class IBestMoveCalculator {
    public:
        virtual PossibleMove getBestMove(Grid& grid) = 0;
        virtual ~IBestMoveCalculator(){};
};
#endif