#ifndef GAME_HPP
#define GAME_HPP

#include "Grid.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "PossibleMove.hpp"
#include <vector>

class Game{
    private:

    public:
        int SizeX;
        int SizeY;
        Grid CurrentMap;
        std::vector<Player> Players;
        std::vector<Field> Fields;

        Game(int sizeX, int sizeY, Grid currentMap, std::vector<Player> players, std::vector<Field> fields) 
            : SizeX(sizeX), SizeY(sizeY), CurrentMap(currentMap), Players(players), Fields(fields) {};

        inline uint8_t getField(int x, int y, Grid grid){
            return grid.MapArray[x][y];
        }

        std::vector<PossibleMove> getPossibleMoves(Player player, Grid grid);

        bool isFieldOccupied(Field field, Grid grid);

        inline Field& getField(int x, int y){
            return Fields[(y*SizeX) + x];
        }

        inline Field& getField(int id){
            return Fields[id];
        }
};
#endif