#include "../../include/game/BestMoveCalculator.hpp"

int BestMoveCalculator::evaluateBoard(Grid grid, Player player, Player opponent) {
    int score = 0;
    // check if this player has won

    // check if opponent has won
    if (this->checkLine(4, grid, opponent)) {
        score = this->_min;
        return score;
    }

    if (this->checkLine(4, grid, player)) {
        score = this->_max;
        return score;
    }

    // check if opponent is about to win
    if (this->checkLine(3, grid, opponent)) {
        score -= 10;
    }

    // check opponent line with two (with gravity)
    if (this->checkLine(2, grid, opponent)) {
        score -= 3;
    }

    // check if this player has a line of two (with gravity)
    if (this->checkLine(2, grid, player)) {
        score += 2;
    }

    // check if this player has  line of three (with gravity)
    if (this->checkLine(3, grid, player)) {
        score += 5;
    }

    return score;
}

bool BestMoveCalculator::checkLine(int numberOfStones, Grid grid, Player player) {
    int lineSize = 4;
    // vedrticalCheck 
    for (int y = 0; y < this->_game.SizeY-3 ; y++ ){
        for (int x = 0; x < this->_game.SizeX; x++){
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
    for (int x = 0; x < this->_game.SizeX-3; x++ ){
        for (int y = 0; y < this->_game.SizeY; y++){
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
    for (int x=3; x < this->_game.SizeX; x++){
        for (int y=0; y < this->_game.SizeY-3; y++){
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
    for (int x=3; x < this->_game.SizeX; x++){
        for (int y=3; y < this->_game.SizeY; y++){
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

PossibleMove BestMoveCalculator::minimax(Grid grid, int depth, int alpha, int beta, bool isMax) {
    bool draw = this->_game.getPossibleMoves(this->_player, grid).size() == 0;

    if (draw) {
        PossibleMove pm;
        pm.Score = 0;
        return pm;
    }

    if (depth == 0 
        || this->checkLine(4, grid, this->_player)
        || this->checkLine(4, grid, this->getOpponent(this->_player))) {
            PossibleMove pm;
            pm.Score = this->evaluateBoard(grid, this->_player, this->getOpponent(this->_player));
            return pm;
    }

    if (isMax) {
        std::vector<PossibleMove> possibleMoves = this->_game.getPossibleMoves(this->_player, grid);
        PossibleMove maxMove = possibleMoves.at(rand() % possibleMoves.size()); 
        maxMove.Score = this->_min;
        for (PossibleMove move : possibleMoves) {
            int score = minimax(move.AfterGrid, depth - 1, alpha, beta, false).Score;
            if(score > maxMove.Score) {
                maxMove = move;
                maxMove.Score = score;
            }
            // pruning
            alpha = std::max(alpha, maxMove.Score);
            if (alpha >= beta) {
                break;
            }
        }
        return maxMove;
    } else {
        std::vector<PossibleMove> possibleMoves = this->_game.getPossibleMoves(this->getOpponent(this->_player), grid);
        PossibleMove minMove = possibleMoves.at(rand() % possibleMoves.size()); 
        minMove.Score = this->_max;
        for (PossibleMove move : possibleMoves) {
            int score = minimax(move.AfterGrid, depth - 1, alpha, beta, true).Score;
            if(score < minMove.Score) {
                minMove = move;
                minMove.Score = score;
            }
            // pruning
            beta = std::min(beta, minMove.Score);
            if (alpha >= beta) {
                break;
            }
        }
        return minMove;
    }
}

Player& BestMoveCalculator::getOpponent(Player& p) {
    for(Player& player : this->_game.Players) {
        if (player.Id != p.Id) {
            return player;
        }
    }

    // fallback
    return this->_game.Players[0];
}