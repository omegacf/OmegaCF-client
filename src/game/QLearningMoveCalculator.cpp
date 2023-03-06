#include "../../include/game/QLearningMoveCalculator.hpp"

QLearningMoveCalculator::QLearningMoveCalculator(Player player): _player(player) {
    Network net;
    this->_netAgent = new NetworkAgent(net);
    this->_netAgent->load();
    this->_netAgent->setMode(NetworkMode::Production);
    this->_agent = new QLearningAgent(player, this->_netAgent, 0.0);
}

PossibleMove QLearningMoveCalculator::getBestMove(Grid& grid) {
    return this->_agent->chooseAction(grid);
}

QLearningMoveCalculator::~QLearningMoveCalculator() {
    delete this->_netAgent;
    delete this->_agent;
}