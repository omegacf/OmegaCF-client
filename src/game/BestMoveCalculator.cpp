#include "../../include/game/BestMoveCalculator.hpp"

int BestMoveCalculator::evaluateBoard(Grid grid, Player player, Player opponent) {
    int score = 0;
    // check if this player is about to win
    if (this->checkLine(4, false, grid, player)) {
        score = std::numeric_limits<int>::max();;
        return score;
    }

    // check if opponent is about to win
    if (this->checkLine(3, true, grid, opponent)) {
        score -= 100;
    }

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

    // check opponent line with two (with gravity)
    if (this->checkLine(2, true, grid, opponent)) {
        score -= 3;
    }

    // check if this player has a line of two (with gravity)
    if (this->checkLine(2, true, grid, player)) {
        score += 2;
    }

    // check if this player has  line of three (with gravity)
    if (this->checkLine(3, true, grid, player)) {
        score += 5;
    }

    return score;
}

bool BestMoveCalculator::checkLine(int numberOfStones, bool gravity, Grid grid, Player player) {
    int lineSize = 4;
    // horizontalCheck 
    for (int y = 0; y < this->_game.SizeY-3 ; y++ ){
        for (int x = 0; y < this->_game.SizeX; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < numberOfStones && brk; s++) {
               if(grid.getStone(x + s, y) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x + s, y) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                       if(gravity) {
                           Field f = this->_game.getField(x + s, y);
                           if(this->_game.hasNeighbour(f, 4)) {
                               Field neighbour = this->_game.getNeighbour(f, 4);
                               if(grid.getStone(neighbour.X, neighbour.Y) == 0) {
                                   brk = true;
                               }
                           }
                       }
                   } else {
                       brk = true;
                   }
               }

            }
            return !brk;          
        }
    }

    // verticalCheck
    for (int x = 0; x < this->_game.SizeX-3; x++ ){
        for (int y = 0; y < this->_game.SizeY; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < numberOfStones && brk; s++) {
               if(grid.getStone(x, y + s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x, y + s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                       if(gravity) {
                           Field f = this->_game.getField(x, y + s);
                           if(this->_game.hasNeighbour(f, 4)) {
                               Field neighbour = this->_game.getNeighbour(f, 4);
                               if(grid.getStone(neighbour.X, neighbour.Y) == 0) {
                                   brk = true;
                               }
                           }
                       }
                   } else {
                       brk = true;
                   }
               }

            }
            return !brk;
        }
    }
    // ascendingDiagonalCheck 
    for (int x=3; x < this->_game.SizeX; x++){
        for (int y=0; y < this->_game.SizeY-3; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < numberOfStones && brk; s++) {
               if(grid.getStone(x - s, y + s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x - s, y + s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                       if(gravity) {
                           Field f = this->_game.getField(x - s, y + s);
                           if(this->_game.hasNeighbour(f, 4)) {
                               Field neighbour = this->_game.getNeighbour(f, 4);
                               if(grid.getStone(neighbour.X, neighbour.Y) == 0) {
                                   brk = true;
                               }
                           }
                       }
                   } else {
                       brk = true;
                   }
               }

            }
            return !brk;
        }
    }
    // descendingDiagonalCheck
    for (int x=3; x < this->_game.SizeX; x++){
        for (int y=3; y < this->_game.SizeY; y++){
            bool brk = false;
            int emptyCounter = 0;
            for(int s = 0; s < numberOfStones && brk; s++) {
               if(grid.getStone(x - s, y - s) != player.Id) {
                   // check if field is free
                   if(grid.getStone(x - s, y - s) == 0 && emptyCounter < (lineSize-numberOfStones)) {
                       emptyCounter++;
                       if(gravity) {
                           Field f = this->_game.getField(x - s, y - s);
                           if(this->_game.hasNeighbour(f, 4)) {
                               Field neighbour = this->_game.getNeighbour(f, 4);
                               if(grid.getStone(neighbour.X, neighbour.Y) == 0) {
                                   brk = true;
                               }
                           }
                       }
                   } else {
                       brk = true;
                   }
               }

            }
            return !brk;
        }
    }
    return false;
}


/*
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
            && grid.getStone(i, j+1) == player.Id 
            && grid.getStone(i, j+2) == player.Id 
            && grid.getStone(i, j+3) == player.Id){
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
            && grid.getStone(i-1, j-1) == player.Id 
            && grid.getStone(i-2, j-2) == player.Id 
            && grid.getStone(i-3, j-3) == player.Id) {
                return true;
            }
        }
    }
    return false;
}
*/