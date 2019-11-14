#include "../../include/game/Game.hpp"

std::array<int, 2> getNeighbourCoordinates(Field f, uint8_t direction){
    switch (direction)
    {
        case 0:
            return {{f.X, f.Y-1}};
        case 1:
            return {{f.X + 1, f.Y + 1}};
        case 2:
            return {{f.X+1, f.Y}};
        case 3:
            return {{f.X+1, f.Y+1}};
        case 4:
            return {{f.X, f.Y+1}};
        case 5:
            return {{f.X-1, f.Y+1}};
        case 6: 
            return {{f.X-1, f.Y}};
        case 7:
            return {{f.X-1, f.Y-1}};
        default:
            return {{f.X, f.Y}};
    }
}

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

std::vector<PossibleMove> Game::getPossibleMoves(Player player, Grid grid){
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