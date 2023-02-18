#include "../../include/game/RandomMoveCalculator.hpp"

PossibleMove RandomMoveCalculator::getBestMove(Grid& grid) {
    std::vector<PossibleMove> possibleMoves = this->_game.getPossibleMoves(this->_player, grid);
    // we can just assume that there is a possible move. Otherwise there would be an error either in the server or in our algorithm
    return possibleMoves.at(rand() % possibleMoves.size());
}