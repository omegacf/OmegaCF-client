#include "../../include/ml/agent.hpp"

PossibleMove Agent::chooseAction(Grid& grid) {
    std::vector<PossibleMove> possibleMoves = this->_game.getPossibleMoves(this->_player, grid);

    if (static_cast<double>(rand()) / RAND_MAX <= this->_expRate) {
        return possibleMoves.at(rand() % possibleMoves.size());
    } else {
        int valueMax = -999;
        int index = 0;
        for(int i = 0; possibleMoves.size(); ++i) {
            std::string hash = possibleMoves.at(i).AfterGrid.getHash();

            std::map<std::string, std::vector<double>>::iterator it = this->_Q.find(hash);
            if (it != this->_Q.end()) {
                index = this->getMaxAction(it);
            }
        }
        return possibleMoves[index];
    }
}

void Agent::updateQ(std::string state, uint8_t action, std::string newState, double reward) {
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

int Agent::getMaxAction(std::map<std::string, std::vector<double>>::iterator& state) {
    std::vector<double>::iterator it = std::max_element(state->second.begin(), state->second.end());
    return std::distance(state->second.begin(), it);
}