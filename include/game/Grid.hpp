#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <iostream>

class Grid{
    public:
        std::vector<std::vector<uint8_t>> MapArray;

        inline uint8_t& getStone(int x, int y){
            return MapArray[x][y];
        }

        Grid(std::vector<std::vector<uint8_t>> mapArray) : MapArray(mapArray) {};
        friend std::ostream& operator << (std::ostream &out, const Grid &g){
            for(std::vector<uint8_t> iv : g.MapArray){
                for(uint8_t i : iv){
                    out << c << " ";
                }
                out << std::endl;
            }
            return out;
        };
};
#endif
