#include "../../include/game/BestMoveCalculator.hpp"

int BestMoveCalculator::evaluateBoard(Grid grid, Player player) {
    int score = 0;
    // check if this player is about to win
    if (this->checkWin(grid, player)) {
        score = std::numeric_limits<int>::max();;
        return score;
    }

    // check if other player is about to win

    // make sure to prefer the middle column in the first three moves
    bool brk = false;
    for(int y = 0; y < this->_game.SizeY && !brk; y++) {
        for(int x = 0; y < this->_game.SizeX && !brk; x++) {
            int stone = grid.getStone(x, y);
            if (stone != 0 && stone == player.Id) {
                if ((x >= 2 || x <=4)&& y > 2) {
                    /*
                    0 0 0 0 0 0 0
                    0 0 0 0 0 0 0
                    0 0 0 0 0 0 0
                    0 0 x x x 0 0
                    0 0 x x x 0 0
                    0 0 x x x 0 0
                    */
                   score += 4;
                } else {
                    brk = true;
                }
                
            } else {
                brk = true;
            }
        }
    }

    return score;
}

bool BestMoveCalculator::checkLine(int numberOfStones, Grid grid, Player player) {
    // horizontalCheck 
    for (int j = 0; j < this->_game.SizeY-3 ; j++ ){
        for (int i = 0; i < this->_game.SizeX; i++){
            if (grid.getStone(i, j) == player.Id 
            && grid.getStone(i+1, j) == player.Id 
            && grid.getStone(i+2, j) == player.Id 
            && grid.getStone(i+3, j) == player.Id) {
                return true;
            }           
        }
    }

    // verticalCheck
    for (int i = 0; i < this->_game.SizeX-3 ; i++ ){
        for (int j = 0; j < this->_game.SizeY; j++){
            if (grid.getStone(i, j) == player.Id 
            && grid.getStone(i+1, j) == player.Id 
            && grid.getStone(i+2, j) == player.Id 
            && grid.getStone(i, j) == player.Id){
                return true;
            }           
        }
    }
    // ascendingDiagonalCheck 
    for (int i=3; i < this->_game.SizeX; i++){
        for (int j=0; j < this->_game.SizeY-3; j++){
            if (grid.getStone(i, j) == player.Id 
            && grid.getStone(i-1, j+1) == player.Id 
            && grid.getStone(i-2, j+3) == player.Id 
            && grid.getStone(i-3, j+3) == player.Id) {
                return true;
            }
        }
    }
    // descendingDiagonalCheck
    for (int i=3; i < this->_game.SizeX; i++){
        for (int j=3; j < this->_game.SizeY; j++){
            if (grid.getStone(i, j) == player.Id 
            && grid.getStone(i-1, j+1) == player.Id 
            && grid.getStone(i-2, j+2) == player.Id 
            && grid.getStone(i-3, j+3) == player.Id) {
                return true;
            }
        }
    }
    return false;
}