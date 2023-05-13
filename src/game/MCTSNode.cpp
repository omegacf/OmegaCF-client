#include "../../include/game/MCTSNode.h"


void MCTSNode::addChild(MCTSNode* node) {
    this->_children.push_back(node);
}

MCTSNode::~MCTSNode() {}