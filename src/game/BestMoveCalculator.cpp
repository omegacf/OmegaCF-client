#include "../../include/game/BestMoveCalculator.hpp"

int BestMoveCalculator::evaluateBoard(Grid& grid, Player player, Player opponent) {
    int score = 0;

    if (Game::checkLine(4, grid, player).size()) {
        score += 1000;
    }

    // 3
    // 100 for 2 empty Neighbours
    // 40 for 1 empty neighbour
    // 0 for 0 empty neighbours
    std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> threeEvaluation = Game::checkLine(3, grid, player);
    for(auto& element : threeEvaluation) {
        int emptyNeighbours = this->_countFreeStones(std::get<0>(element), std::get<1>(element), std::get<2>(element), grid, player, 3);
        if (emptyNeighbours == 1)
            score += 40;
        else if (emptyNeighbours == 2)
            score += 100;
    }

    // 2
    // 10 for 2 empty Neighbours
    // 4 for 1 empty neighbour
    // 0 for 0 empty neighbours
    std::vector<std::tuple<LineType, std::pair<int, int>, std::pair<int, int>>> twoEvaluation = Game::checkLine(2, grid, player);
    for(auto& element : twoEvaluation) {
        int emptyNeighbours = this->_countFreeStones(std::get<0>(element), std::get<1>(element), std::get<2>(element), grid, player, 2);
        if (emptyNeighbours == 1)
            score += 4;
        else if (emptyNeighbours == 2)
            score += 10;
    }

    /*
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
*/
    return score;
}

int BestMoveCalculator::_countFreeStones(LineType type, std::pair<int, int>& begin, std::pair<int, int>& end, Grid& grid, Player& player, int numberOfStones) {
    int result = 0;
    // make sure that begin is left and end is right
    std::pair<int, int> tBegin, tEnd;
    tBegin = std::min(begin.first, end.first) == begin.first?begin:end;
    tEnd = std::max(begin.first, end.first) == end.first?end:begin;
    switch (type)
    {
        case LineType::Horizontal:
            if(tBegin.first > 0 && grid.getStone(tBegin.first-1, tBegin.second) == 0)
                ++result;
            if(tEnd.first < grid.SizeX-1 && grid.getStone(tEnd.first+1, tEnd.second) == 0)
                ++result;
            break;
        case LineType::Vertical:
            // make sure that begin is bottom and end is up
            tBegin = std::min(begin.second, end.second) == begin.second?begin:end;
            tEnd = std::max(begin.second, end.second) == end.second?end:begin;

            if(begin.second > 0 && grid.getStone(begin.first, begin.second-1) == 0)
                ++result;
            if(tEnd.second < grid.SizeY-1 && grid.getStone(tEnd.first, tEnd.second+1) == 0)
                ++result;
            break;     
        case LineType::AscendingDiagonal:
            if((tBegin.first>0 && tBegin.second<grid.SizeY-1) && grid.getStone(tBegin.first-1, tBegin.second+1) == 0)
                ++result;
            if((tEnd.second>0 && tEnd.first<grid.SizeX-1) && grid.getStone(tEnd.first+1, tEnd.second-1) == 0)
                ++result;
            break;    
        case LineType::DescendingDiagonal:
            if((tBegin.first>0 && tBegin.second>0) && grid.getStone(tBegin.first-1, tBegin.second-1) == 0)
                ++result;
            if((tEnd.first<grid.SizeX-1 && tEnd.second<grid.SizeY-1) && grid.getStone(tEnd.first+1, tEnd.second+1) == 0)
                ++result;
            break;       
        default:
            break;
    }
    return result;
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
            Player* player = &this->_player;
            if (!isMax)
                player = &this->getOpponent(this->_player);
            pm.Score = this->evaluateBoard(grid, this->_player, *player);
            return pm;
    }
    
    if (Game::checkLine(4, grid, this->_player).size()) {
        PossibleMove pm;
        pm.Score = this->_max;
        return pm;
    }

    if (Game::checkLine(4, grid, this->getOpponent(this->_player)).size()) {
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