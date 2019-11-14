#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <iostream>

class Grid{
    public:
        std::vector<std::vector<uint8_t>> MapArray;

        Grid(){};
        Grid(std::vector<std::vector<uint8_t>> mapArray) : MapArray(mapArray){};

        inline uint8_t& getStone(int x, int y){
            return MapArray[y][x];
        }

        friend std::ostream& operator << (std::ostream &out, const Grid &g){
            for(std::vector<uint8_t> iv : g.MapArray){
                for(uint8_t i : iv){
                    out << (int)i << " ";
                }
                out << std::endl;
            }
            return out;
        };
};
#endif
