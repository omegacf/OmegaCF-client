#ifndef POSSIBLEMOVE_HPP
#define POSSIBLEMOVE_HPP

#include "Field.hpp"
#include "Grid.hpp"

class PossibleMove{
    public:
        PossibleMove(int move, Grid grid) : 
            Move(move), AfterGrid(grid){};
        int Move;
        Grid AfterGrid;
};
#endif