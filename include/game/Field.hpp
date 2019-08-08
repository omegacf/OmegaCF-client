#ifndef FIELD_HPP
#define FIELD_HPP
class Field{
    int id;
    int Neighbours[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    int Rating;
    int X, Y;
    Field(int x, int y) : X(x), Y(y){}
};
#endif