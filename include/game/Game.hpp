#ifndef GAME_HPP
#define GAME_HPP

#include "Grid.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "PossibleMove.hpp"
#include <vector>
#include <array>

class Game{
    private:
        std::array<int, 2> getNeighbourCoordinates(Field f, uint8_t direction);

    public:
        int SizeX;
        int SizeY;
        Grid CurrentMap;
        std::vector<Player> Players;
        std::vector<Field> Fields;

        Game(int sizeX, int sizeY, Grid currentMap, std::vector<Player> players, std::vector<Field> fields) 
            : SizeX(sizeX), SizeY(sizeY), CurrentMap(currentMap), Players(players), Fields(fields) {};

        Game() : SizeX(0), SizeY(0) {};

        void setStone(Player player, int x, Grid& grid);

        std::vector<PossibleMove> getPossibleMoves(Player player, Grid grid);

        bool isFieldOccupied(Field field, Grid grid);

        inline Field& getField(int x, int y){
            return Fields[(y*SizeX) + x];
        }

        inline Field& getNeigbour(Field f, uint8_t direction){
            std::array<int, 2> coordinatesArray = this->getNeighbourCoordinates(f, direction);
            return this->getField(coordinatesArray[0], coordinatesArray[1]);
        }

        inline bool hasNeighbour(Field f, uint8_t direction){
            std::array<int, 2> coordinatesArray = this->getNeighbourCoordinates(f, direction);
            return(coordinatesArray[0] >= 0 && coordinatesArray[0] <= this->SizeX
                    && coordinatesArray[1] >= 0 && coordinatesArray[1] <= this->SizeY);
        }
};
#endif