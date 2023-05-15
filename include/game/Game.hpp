#ifndef GAME_HPP
#define GAME_HPP

#include "Grid.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "PossibleMove.hpp"
#include "../util/enums.hpp"
#include <vector>
#include <array>
#include <tuple>
#include <utility>


class Game{

    public:
        Grid CurrentMap;
        std::vector<Player> Players;

        Game(int sizeX, int sizeY, Grid currentMap, std::vector<Player> players) 
            : CurrentMap(currentMap), Players(players) {};
        Game(){};

        Player getPlayer(int8_t playerNumber);
        void reset();

        static void setStone(Player player, int x, Grid& grid);
        static std::vector<PossibleMove> getPossibleMoves(Player& player, Grid& grid);
        static std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> checkLine(int numberOfStones, Grid& grid, Player player);
        Player getNextPlayer(int8_t playerNumber);
        Player getNextPlayer(Player& currentPlayer);
};
#endif