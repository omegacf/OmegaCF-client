#include "../../include/game/Game.hpp"

void Game::setStone(Player player, int x, Grid& grid){
    int y = 0;
    for(int i = 0; i < this->SizeY; i++){
        if(grid.getStone(x, y) == 0){
            y++;
        }else{
            break;
        }
    }
    grid.getStone(x, y-1) = player.Id;
}

std::vector<PossibleMove> Game::getPossibleMoves(Player& player, Grid& grid){
    std::vector<PossibleMove> moves;
    for(int x = 0; x < this->SizeX; x++){
        if(grid.getStone(x, 0) == 0){
            Grid cgrid(grid);
            this->setStone(player, x, cgrid);
            moves.push_back(PossibleMove(x, cgrid));
        }
    }
    return moves;
}

Player Game::getPlayer(int8_t playerNumber){
    for (Player player : this->Players) {
        if(player.Id == playerNumber) {
            return player;
        }
    }
    // fallback
    return Player();
}