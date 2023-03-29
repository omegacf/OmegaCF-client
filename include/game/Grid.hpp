#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class Grid{
    private:
        std::string ternary_to_hexadecimal(const std::string& ternary);
    public:
        std::vector<std::vector<uint8_t>> MapArray;
        uint8_t SizeX, SizeY;

        Grid():SizeX(0), SizeY(0){};
        Grid(std::vector<std::vector<uint8_t>> mapArray, uint8_t sizeX, uint8_t sizeY) 
            : MapArray(mapArray), SizeX(sizeX), SizeY(sizeY){};

        std::string getHash();

        inline uint8_t& getStone(int x, int y){
            return MapArray[y][x];
        }

        friend std::ostream& operator << (std::ostream &out, const Grid &g){
            for(auto&& iv : g.MapArray){
                for(uint8_t i : iv){
                    out << (int)i << " ";
                }
                out << std::endl;
            }
            return out;
        };
};
#endif
