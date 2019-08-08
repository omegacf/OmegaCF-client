#ifndef GAMEFACTORY_HPP
#define GAMEFACTORY_HPP

#include "../game/Game.hpp"
#include <string>

class GameFactory{
    public:
        static Game create(int sizeX, int sizeY, int playerNumber);
};
#endif