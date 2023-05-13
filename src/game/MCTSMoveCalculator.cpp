#include "../../include/game/MCTSMoveCalculator.hpp"

// return that is leaf and non terminal, and has highest UCB value
MCTSNode* MCTSMoveCalculator::_selection(MCTSNode& node) {
    // Breadth-first search
    std::queue<MCTSNode*> Q;
    MCTSNode* nodeToExpand = &node;
    bool isTerminal = false;
    while (!isTerminal) {
        float maxUCB = std::numeric_limits<float>::min();
        unsigned int index = 0;
        std::vector<MCTSNode*> children = nodeToExpand->getChildren();
        if (!children.size()) {
            for(MCTSNode* child : children) {
                float ucb = 0;

                // prevent division zero
                if (child->getVisits() == 0) {
                   ucb = std::numeric_limits<float>::max();
                } else {
                   ucb = this->_calcUCB(*child);
                }

                if (ucb > maxUCB) {
                    maxUCB = ucb;
                    nodeToExpand = child;
                }
            }
        } else {
            isTerminal = true;
        }
    }

    return nodeToExpand;
    

    Q.push(&(node));
    while (Q.size()) {
        MCTSNode* v = Q.front();
        Q.pop();
        // check if v is a leaf node
        if (!v->getChildren().size()) {
            // v is leaf node
            // check if v is terminal (no possible moves) 
            Player nextPlayer = this->_game->getNextPlayer(v->currentPlayer);
            Grid nodeGrid = node.getGrid();
            if (Game::getPossibleMoves(nextPlayer, nodeGrid).size()) {
                // found possible node for expansion
                float ucb = this->_calcUCB(*v);
                if (ucb > maxUCB) {
                   maxUCB = ucb;
                   nodeToExpand = v; 
                }
            }
        } else {
            for(MCTSNode* child : v->getChildren()) {
                Q.push(child);
            }
        }
    }

    return nodeToExpand;
}

// just call expansion if node is not a terminal one
MCTSNode* MCTSMoveCalculator::_expansion(MCTSNode& node) {
    Player nextPlayer = this->_game->getNextPlayer(node.currentPlayer);
    Grid nodeGrid = node.getGrid();
    std::vector<PossibleMove> possibleMoves = Game::getPossibleMoves(nextPlayer, nodeGrid);

    // no need to check if there are moves
    for(PossibleMove& move : possibleMoves) {
        MCTSNode* newNode = new MCTSNode(move.AfterGrid, &node, 0, 0.0f);
        node.addChild(newNode);
    }

    // select random child
    return node.getChildren().at(rand() % node.getChildren().size());
}

// just call simulation if node is not a terminal one
// TODO: make function work, if node is a terminal one
float MCTSMoveCalculator::_simulation(MCTSNode& node) {
    bool gameOver = false;
    float score = 0;
    Grid nodeGrid = node.getGrid();
    // Player nextPlayer = this->_game->getNextPlayer(node.currentPlayer);
    std::vector<PossibleMove> moves = Game::getPossibleMoves(nextPlayer, nodeGrid);
    while (gameOver) {
        // check if game is in a terminal state
        // select random move
        PossibleMove randMove = moves.at(rand() % moves.size());
        // check if somebody has won
        if (Game::checkLine(4, randMove.AfterGrid, nextPlayer).size()) {
            gameOver = true;
            // somebody has won
            if (nextPlayer.Id == this->_player.Id) {
                score = WIN_SCORE;
            }
        }

        Grid nodeGrid = randMove.AfterGrid;
        Player nextPlayer = this->_game->getNextPlayer(node.currentPlayer);
        std::vector<PossibleMove> moves = Game::getPossibleMoves(nextPlayer, nodeGrid);
        if (!moves.size()) {
            // draw
            gameOver = true;
            score = DRAW_SCORE;
        }
    }

    return score;
}

void MCTSMoveCalculator::_backpropagation(MCTSNode& node, float value){
    MCTSNode* n = &node;
    do{
        n->updateScore(value);
        n = n->getParent();
    }while(n->getParent() != nullptr);
}

PossibleMove MCTSMoveCalculator::getBestMove(Grid& grid) {
    // create root node
    MCTSNode* root = new MCTSNode(grid, nullptr, 0, 0);
    for(unsigned int i = 0; i<this->iterations; ++i) {
        // selection

        // dont expand node if it was never sampled before
    }
}

void MCTSMoveCalculator::_clearTree(MCTSNode& root) {
    std::queue<MCTSNode*> Q;
    Q.push(&root);
    while (Q.size()) {
        MCTSNode* node = Q.front();
        Q.pop();
        std::vector<MCTSNode*> children = node->getChildren();
        if (!children.size()) {
            // leaf node found -> delete it
            delete node;
        } else {
            for (MCTSNode* child : children) {
                Q.push(child);
            }
        }
    }
}

float MCTSMoveCalculator::_calcUCB(MCTSNode& node) {
    return node.getScore() + (2*std::sqrt(std::log(node.getParent()->getVisits()) / node.getVisits()));
}