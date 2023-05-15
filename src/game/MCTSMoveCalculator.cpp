#include "../../include/game/MCTSMoveCalculator.hpp"

// return that is leaf and non terminal, and has highest UCB value
MCTSNode* MCTSMoveCalculator::_selection(MCTSNode& node) {
    // Breadth-first search
    std::queue<MCTSNode*> Q;
    MCTSNode* nodeToExpand = &node;
    bool isTerminal = false;
    while (!isTerminal) {
        float maxUCB = -std::numeric_limits<float>::max();
        std::vector<std::pair<int, MCTSNode*>> children = nodeToExpand->getChildren();
        if (children.size()) {
            for(std::pair<int, MCTSNode*>& child : children) {
                float ucb = 0;

                // prevent division zero
                if (child.second->getVisits() == 0) {
                   ucb = std::numeric_limits<float>::max();
                } else {
                   ucb = this->_calcUCB(*child.second);
                }

                if (ucb > maxUCB) {
                    maxUCB = ucb;
                    nodeToExpand = child.second;
                }
            }
        } else {
            isTerminal = true;
        }
    }

    return nodeToExpand;
}

MCTSNode* MCTSMoveCalculator::_expansion(MCTSNode& node) {
    Player nextPlayer = node.currentPlayer;
    Grid nodeGrid = node.getGrid();
    std::vector<PossibleMove> possibleMoves = Game::getPossibleMoves(nextPlayer, nodeGrid);

    // check if node is a terminal one
    if (!possibleMoves.size() || Game::checkLine(4, nodeGrid, nextPlayer).size() != 0) {
        return &node;
    }
    // no need to check if there are moves
    for(PossibleMove& move : possibleMoves) {
        MCTSNode* newNode = new MCTSNode(move.AfterGrid, &node, 0, 0.0f);
        newNode->currentPlayer = this->_game->getNextPlayer(nextPlayer);
        node.addChild(move.Move, newNode);
    }

    // select random child
    return node.getChildren().at(rand() % node.getChildren().size()).second;
}

// just call simulation if node is not a terminal one
float MCTSMoveCalculator::_simulation(MCTSNode& node) {
    bool gameOver = false;
    float score = 0;
    Grid nodeGrid = node.getGrid();
    // same as next player, because there are only two
    Player prevPlayer = this->_game->getNextPlayer(node.currentPlayer);
    std::vector<PossibleMove> moves = Game::getPossibleMoves(node.currentPlayer, nodeGrid);

    // check if node is a terminal one
    if (Game::checkLine(4, nodeGrid, prevPlayer).size()) {
        if (prevPlayer.Id == this->_player.Id) {
            return WIN_SCORE;
        } else {
            return LOOSE_SCORE;
        }
    }
    if (!moves.size()) {
        return DRAW_SCORE;
    }

    Player player = node.currentPlayer;
    
    while (!gameOver) {
        // select random move
        PossibleMove randMove = moves.at(rand() % moves.size());
        // std::cout << randMove.AfterGrid << std::endl;
        // check if somebody has won
        if (Game::checkLine(4, randMove.AfterGrid, player).size()) {
            gameOver = true;
            // somebody has won
            if (player.Id == this->_player.Id) {
                score = WIN_SCORE;
            } else {
                score = LOOSE_SCORE;
            }
            break;
        }

        Grid nodeGrid = randMove.AfterGrid;
        player = this->_game->getNextPlayer(player);
        moves = Game::getPossibleMoves(player, nodeGrid);
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
    }while(n != nullptr);
}

PossibleMove MCTSMoveCalculator::getBestMove(Grid& grid) {
    Debug::printLine("Run MCTS");
    // create root node
    MCTSNode* root = new MCTSNode(grid, nullptr, 0, 0);
    root->currentPlayer = this->_player;
    for(unsigned int i = 0; i<this->iterations; ++i) {
        // selection
        MCTSNode* selectedNode = this->_selection(*root);

        // check if node was visited before
        // if yes, expand it, if not, just carry on with the simulation
        if (selectedNode->getVisits()) {
            // expand it
            selectedNode = this->_expansion(*selectedNode);
        }

        // simulation
        float score = this->_simulation(*selectedNode);

        // backpropagation
        this->_backpropagation(*selectedNode, score);
    }

    // select move where node has highest ucb
    int visits = 0;
    PossibleMove move;

    for(std::pair<int, MCTSNode*> child : root->getChildren()) {
        std::cout << "Move: " << child.first << " | Wins: " << child.second->getScore() << " | Visits: " << child.second->getVisits() << std::endl;
        if (child.second->getVisits() > visits) {
            visits = child.second->getVisits();
            move = PossibleMove(child.first, child.second->getGrid());
        }
    }
    this->_clearTree(*root);
    return move;
}

void MCTSMoveCalculator::_clearTree(MCTSNode& root) {
    std::queue<MCTSNode*> Q;
    Q.push(&root);
    while (Q.size()) {
        MCTSNode* node = Q.front();
        Q.pop();
        std::vector<std::pair<int, MCTSNode*>> children = node->getChildren();
        if (!children.size()) {
            // leaf node found -> delete it
            delete node;
        } else {
            for (std::pair<int, MCTSNode*>& child : children) {
                Q.push(child.second);
            }
        }
    }
}

float MCTSMoveCalculator::_calcUCB(MCTSNode& node) {
    return (node.getScore()/(float)node.getVisits()) + (2*std::sqrt(std::log(node.getParent()->getVisits()) / node.getVisits()));
}