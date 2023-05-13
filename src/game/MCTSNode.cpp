#include "../../include/game/MCTSNode.h"


void MCTSNode::addChild(int move, MCTSNode* node) {
    this->_children.push_back(std::make_pair(move, node));
}

MCTSNode::~MCTSNode() {}