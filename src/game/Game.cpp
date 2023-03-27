#include "../../include/game/Game.hpp"

void Game::reset() {
    std::vector<std::vector<uint8_t>> mapArray(this->CurrentMap.SizeY, std::vector<uint8_t>(this->CurrentMap.SizeX, 0));
    this->CurrentMap.MapArray = mapArray;
}

void Game::setStone(Player player, int x, Grid& grid){
    int y = 0;
    for(int i = 0; i < grid.SizeY; ++i){
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
    for(int x = 0; x < grid.SizeX; x++){
        if(grid.getStone(x, 0) == 0){
            Grid cgrid(grid);
            Game::setStone(player, x, cgrid);
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

std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> Game::checkLine(int numberOfStones, Grid& grid, Player player) {
    std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> result;
    // verticalCheck 
    for (int x = 0; x < grid.SizeX; x++){
        for (int y = 0; y < grid.SizeY-(numberOfStones-1) ; y++){
            int amountOfStonesInARow = 0;
            if(y==0 || grid.getStone(x, y-1) != player.Id) {
                for(int s = 0; s < (grid.SizeY - y); s++) {
                    if(grid.getStone(x, y + s) == player.Id) {
                        amountOfStonesInARow++;
                    } else {
                        break;
                    }
                }
            }

            if (amountOfStonesInARow == numberOfStones) {
                result.push_back(
                    std::make_tuple<LineType, std::pair<int, int>, std::pair<int, int>>(
                        LineType::Vertical, 
                        std::make_pair(x, y), 
                        std::make_pair(x, y + numberOfStones - 1)
                    )
                );
            }    
        }
    }

    // horizontalCheck
    for (int x = 0; x < grid.SizeX-(numberOfStones-1); ++x){
        for (int y = 0; y < grid.SizeY; ++y){
            int amountOfStonesInARow = 0;
            if(x==0 || grid.getStone(x-1, y) != player.Id) {
                for(int s = 0; s < (grid.SizeX - x); ++s) {
                if(grid.getStone(x + s, y) == player.Id) {
                        amountOfStonesInARow++;
                    } else {
                        break;
                    }
                }
            }

            if (amountOfStonesInARow == numberOfStones) {
                result.push_back(
                    std::make_tuple<LineType, std::pair<int, int>, std::pair<int, int>>(
                        LineType::Horizontal, 
                        std::make_pair(x, y), 
                        std::make_pair(x + numberOfStones - 1, y)
                    )
                );
            }    
        }
    }
    // descendingDiagonalCheck 
    for (int x=0; x < grid.SizeX-(numberOfStones-1); ++x){
        for (int y=0; y < grid.SizeY-(numberOfStones-1); ++y){
            int amountOfStonesInARow = 0;
            if((y==0 || x==0) || grid.getStone(x-1, y-1) != player.Id) {
                for(int s = 0; s < std::min(grid.SizeX - x, grid.SizeY - y); ++s) {
                if(grid.getStone(x + s, y + s) == player.Id) {
                        amountOfStonesInARow++;
                    } else {
                        break;
                    }
                }   
            }

            if (amountOfStonesInARow == numberOfStones) {
                result.push_back(
                    std::make_tuple<LineType, std::pair<int, int>, std::pair<int, int>>(
                        LineType::DescendingDiagonal, 
                        std::make_pair(x, y), 
                        std::make_pair(x + (numberOfStones - 1), y + (numberOfStones - 1))
                    )
                );
            }   
        }
    }
    // ascendingDiagonalCheck
    for (int x=0; x < grid.SizeX-(numberOfStones-1); ++x){
        for (int y=(numberOfStones-1); y < grid.SizeY; ++y){
            int amountOfStonesInARow = 0;
            if((y==(grid.SizeY-1) || x==(grid.SizeX-1)) || grid.getStone(x+1, y+1) != player.Id) {
                for(int s = 0; s < std::min(grid.SizeX - x, y); ++s) {
                if(grid.getStone(x + s, y - s) == player.Id) {
                        amountOfStonesInARow++;
                    } else {
                        break;
                    }
                }
            }

            if (amountOfStonesInARow == numberOfStones) {
                result.push_back(
                    std::make_tuple<LineType, std::pair<int, int>, std::pair<int, int>>(
                        LineType::AscendingDiagonal, 
                        std::make_pair(x, y), 
                        std::make_pair(x + (numberOfStones - 1), y - (numberOfStones - 1))
                    )
                );
            }    
        }
    }
    return result;
}