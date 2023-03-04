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
        Grid CurrentMap;
        std::vector<Player> Players;

        Game(int sizeX, int sizeY, Grid currentMap, std::vector<Player> players) 
            : CurrentMap(currentMap), Players(players) {};
        Game(){};

        Player getPlayer(int8_t playerNumber);

        static void setStone(Player player, int x, Grid& grid);
        static std::vector<PossibleMove> getPossibleMoves(Player& player, Grid& grid);
        static bool checkLine(int numberOfStones, Grid& grid, Player player);
};
#endif