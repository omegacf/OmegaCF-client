#include "../../include/ml/qLearningAgent.hpp"

PossibleMove QLearningAgent::chooseAction(Grid& grid) {
    std::vector<PossibleMove> possibleMoves = Game::getPossibleMoves(this->_player, grid);

    if (static_cast<float>(rand()) / RAND_MAX <= this->_expRate) {

        return possibleMoves.at(rand() % possibleMoves.size());
    } else {
        NetworkOutput output = this->_networkAgent->evalPosition(grid, this->_player.Id);
        // check if action is a possible move
        return this->getActionFromQValues(possibleMoves, output.action);
    }
}

PossibleMove QLearningAgent::getActionFromQValues(std::vector<PossibleMove>& possibleMoves, std::array<float, 7>& qValues) {
    std::array<bool, 7> mask = {0, 0, 0, 0, 0, 0, 0};

    for(PossibleMove& pm : possibleMoves) {
        mask[pm.Move] = 1;
    }
    
    float maxQValue = 0;
    int action = -1;
    bool maxExists = false;

    for(uint8_t i = 0; i < 7; ++i) {
        if(mask[i]){
            if (!maxExists || maxQValue < qValues[i]) {
                maxExists = true;
                maxQValue = qValues[i];
                action = i;
            }
        }
    }
    assert(action >= 0);
    PossibleMove move;
    for(PossibleMove& pm : possibleMoves) {
        if (pm.Move == action) {
            move = pm;
        }
    }
    return move;
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