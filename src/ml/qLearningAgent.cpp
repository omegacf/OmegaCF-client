#include "../../include/ml/qLearningAgent.hpp"

PossibleMove QLearningAgent::chooseAction(Grid& grid) {
    std::vector<PossibleMove> possibleMoves = this->_game->getPossibleMoves(*this->_player, grid);

    if (static_cast<double>(rand()) / RAND_MAX <= this->_expRate) {
        return possibleMoves.at(rand() % possibleMoves.size());
    } else {
        NetworkOutput output = this->_networkAgent->evalPosition(grid, this->_player->Id);
        // get max action
        int action = std::max_element(output.action.begin(), output.action.end()) - output.action.begin();
        
        // check if action is a possible move
        bool isValid = false;
        PossibleMove move;
        for(PossibleMove& pm : possibleMoves) {
            if (pm.Move == action) {
                move = pm;
                isValid = true;
                break;
            }
        }

        // if move is valid, return it. If not take a random one (and hope for the best)
        if (isValid) {
            return move;
        } else {
            return possibleMoves.at(rand() % possibleMoves.size());
        }
    }
}

void QLearningAgent::updateQ(std::string state, uint8_t action, std::string newState, double reward) {
    /*
        firstterm=(1-alpha)*Q[state,action]
        secondterm=gamma*Q[newstate,:].max()
        thirdterm=alpha*(reward+secondterm)
        res=firstterm+thirdterm
        Q[state,action]=res
    */

   // check if state is in Q
   // double firstTerm = (1-this->_alpha) * ;
}