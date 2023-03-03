#include "../../include/game/Game.hpp"

void Game::setStone(Player player, int x, Grid& grid){
    int y = 0;
    for(int i = 0; i < grid.SizeY; i++){
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

bool Game::checkLine(int numberOfStones, Grid& grid, Player player) {
    int lineSize = 4;
    // verticalCheck 
    for (int y = 0; y < grid.SizeY-3 ; y++ ){
        for (int x = 0; x < grid.SizeX; x++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < lineSize && !brk; s++) {
               if(grid.getStone(x, y + s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x, y + s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                   } else {
                       brk = true;
                   }
               }

            }
            if(brk == false) {
                return true;
            }          
        }
    }

    // horizontalCheck
    for (int x = 0; x < grid.SizeX-3; x++ ){
        for (int y = 0; y < grid.SizeY; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < lineSize && !brk; s++) {
               if(grid.getStone(x + s, y) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x + s, y) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                   } else {
                       brk = true;
                   }
               }

            }
            if(!brk) {
                return true;
            }     
        }
    }
    // ascendingDiagonalCheck 
    for (int x=3; x < grid.SizeX; x++){
        for (int y=0; y < grid.SizeY-3; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < lineSize && !brk; s++) {
               if(grid.getStone(x - s, y + s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x - s, y + s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                   } else {
                       brk = true;
                   }
               }

            }
            if(!brk) {
                return true;
            }     
        }
    }
    // descendingDiagonalCheck
    for (int x=3; x < grid.SizeX; x++){
        for (int y=3; y < grid.SizeY; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < lineSize && !brk; s++) {
               if(grid.getStone(x - s, y - s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x - s, y - s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                   } else {
                       brk = true;
                   }
               }

            }
            if(!brk) {
                return true;
            }     
        }
    }
    return false;
}