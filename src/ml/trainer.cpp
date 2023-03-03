#include "../../include/ml/trainer.hpp"

void Trainer::train(int amountGames, QLearningAgent& qAgent, IBestMoveCalculator* opponent) {
    Debug::printLine("Start training...");

    int playerOneWinnings = 0;
    int playerTwoWinnings = 0;
    
    
    for(int i = 0; i < amountGames; ++i) {
        Game game = GameFactory::create(7, 6, 2);
        int playerTurn = rand() % 2 + 1;
        bool gameIsRunning = true;
        while (gameIsRunning) {
            Player player = game.getPlayer(playerTurn);
            int move = -1;
            Grid state(game.CurrentMap);
            int reward = 0;
            if (playerTurn == 1) {
                // qLearning
                move = qAgent.chooseAction(game.CurrentMap).Move;
                game.setStone(player, move, game.CurrentMap);
            } else {
                // Opponent
                move = opponent->getBestMove(game.CurrentMap).Move;
                game.setStone(player, move, game.CurrentMap);
            }
            Grid newState(game.CurrentMap);

            // check if game is over
            if(game.checkLine(4, game.CurrentMap, player)) {
                if(Debug::getFlag()) {
                    std::cout << "Player " << (int)player.Id << " has won" << std::endl;
                }
                // player has won
                if (playerTurn == 1) {
                    reward = 1;
                    playerOneWinnings++;
                } else {
                    reward = 0;
                    playerTwoWinnings++;
                }
                gameIsRunning = false;
            } else {
                // check for draw
                if (game.getPossibleMoves(player, game.CurrentMap).size() == 0) {
                    // draw
                    reward = 0;
                    gameIsRunning = false;
                    Debug::printLine("Draw");
                }
            }
            if (playerTurn == 1) {
                this->_networkAgent->addMemory(1, state, move, newState, reward, !gameIsRunning);
                this->_networkAgent->optimize();
            }
            
            playerTurn = (playerTurn % 2) + 1;
            if(Debug::getFlag()) {
                std::cout << game.CurrentMap << std::endl;
            }
        }
    }
    if(Debug::getFlag()) {
        std::cout << "Player 1 winnings: " << playerOneWinnings << std::endl;
        std::cout << "Player 2 winnings: " << playerTwoWinnings << std::endl;
    } 
}