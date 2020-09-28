#ifndef GAME_HPP
#define GAME_HPP

#include "Grid.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "PossibleMove.hpp"
#include <vector>
#include <array>

class Game{

    public:
        int SizeX;
        int SizeY;
        Grid CurrentMap;
        std::vector<Player> Players;

        Game(int sizeX, int sizeY, Grid currentMap, std::vector<Player> players) 
            : SizeX(sizeX), SizeY(sizeY), CurrentMap(currentMap), Players(players) {};

        Game() : SizeX(0), SizeY(0) {};

        void setStone(Player player, int x, Grid& grid);
        std::vector<PossibleMove> getPossibleMoves(Player player, Grid grid);
        Player getPlayer(int8_t playerNumber);
};
#endif