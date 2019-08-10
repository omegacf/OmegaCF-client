#include "../../include/factory/GameFactory.hpp"

Game GameFactory::create(int sizeX, int sizeY, int playerNumber){
    std::vector<std::vector<uint8_t>> mapArray(sizeY, std::vector<uint8_t>(sizeX, 0));
    Grid grid(mapArray);
    
    //init players
    std::vector<Player> players;
    for(uint8_t i = 1; i<=playerNumber; i++){
        Player p(i);
        players.push_back(p);
    }

    std::vector<Field> fields;
    //init Fields
    for(int y = 0; y < sizeY; y++){
        for(int x = 0; x < sizeX; x++){
            Field field(x, y);
            fields.push_back(field);
        }
    }

    return Game(sizeX, sizeY, grid, players, fields);
}