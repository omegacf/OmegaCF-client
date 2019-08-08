#ifndef POSSIBLEMOVE_HPP
#define POSSIBLEMOVE_HPP

#include "Field.hpp"
#include "Grid.hpp"

class PossibleMove{
    public:
        PossibleMove(Field field, Grid grid) : 
            PossibleField(field), AfterGrid(grid){};
        Field PossibleField;
        Grid AfterGrid;
};
#endif