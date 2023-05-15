#ifndef MCTSNODE_H
#define MCTSNODE_H

#include "Grid.hpp"
#include <vector>
#include "Player.hpp"
#include <utility>

class MCTSNode {
    private:
        Grid _grid;
        MCTSNode* _parent = nullptr;
        std::vector<std::pair<int, MCTSNode*>> _children;
        unsigned int _visits;
        float _score;

    public:
        MCTSNode() {};
        MCTSNode(Grid grid, MCTSNode* parent, unsigned int visits, float score): _grid(grid), _parent(parent), _visits(visits), _score(score) {};

        void addChild(int move, MCTSNode* node);
        // player who is next
        Player currentPlayer;
        inline float getScore() {return this->_score;};
        inline unsigned int getVisits() {return this->_visits;};
        inline MCTSNode* getParent() {return this->_parent;};
        inline std::vector<std::pair<int, MCTSNode*>> getChildren() {return this->_children;};
        inline Grid getGrid() {return this->_grid;};
        inline void updateScore(float score) {
            this->_score += score;
            this->_visits++;
        };

        ~MCTSNode();
};
#endif
