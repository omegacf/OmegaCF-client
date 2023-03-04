#include "../../include/game/BestMoveCalculator.hpp"

int BestMoveCalculator::evaluateBoard(Grid grid, Player player, Player opponent) {
    int score = 0;
    
    if (Game::checkLine(4, grid, player)) {
        score += 100;
    } else if (Game::checkLine(3, grid, player)) {
        score += 5;
    } else if (Game::checkLine(2, grid, player)) {
        score += 2;
    }

    if(Game::checkLine(3, grid, opponent)) {
        score -=4;
    }

    return score;
}

PossibleMove BestMoveCalculator::getBestMove(Grid& grid) {
    return this->minimax(grid, this->_depth);
}

PossibleMove BestMoveCalculator::minimax(Grid& grid, int depth, int alpha, int beta, bool isMax) {
    bool draw = Game::getPossibleMoves(this->_player, grid).size() == 0;

    if (draw) {
        PossibleMove pm;
        pm.Score = 0;
        return pm;
    }

    if (depth == 0) {
            PossibleMove pm;
            pm.Score = this->evaluateBoard(grid, this->_player, this->getOpponent(this->_player));
            return pm;
    }

    if (Game::checkLine(4, grid, this->_player)) {
        PossibleMove pm;
        pm.Score = this->_max;
        return pm;
    }

    if (Game::checkLine(4, grid, this->getOpponent(this->_player))) {
        PossibleMove pm;
        pm.Score = this->_min;
        return pm;
    }

    if (isMax) {
        std::vector<PossibleMove> possibleMoves = Game::getPossibleMoves(this->_player, grid);
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
        std::vector<PossibleMove> possibleMoves = Game::getPossibleMoves(this->getOpponent(this->_player), grid);
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
    for(Player& player : this->_game->Players) {
        if (player.Id != p.Id) {
            return player;
        }
    }

    // fallback
    return this->_game->Players[0];
}