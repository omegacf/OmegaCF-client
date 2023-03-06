#include "../../include/ml/trainer.hpp"

Trainer::Trainer(MoveCalculator mc, NetworkAgent& networkAgent) {
    this->_networkAgent = &networkAgent;
    this->_game = GameFactory::create(7, 6, 2);
    int opponentNumber = (this->_playerNumber % 2) + 1;

    switch (mc)
    {
        case MoveCalculator::MachineLearning:
            this->_bmc = new QLearningMoveCalculator(this->_game.getPlayer(opponentNumber));
            break;
        case MoveCalculator::MinMax:
            this->_bmc = new BestMoveCalculator(&this->_game, this->_game.getPlayer(opponentNumber), 6);
            break;
        case MoveCalculator::Random:
            this->_bmc = new RandomMoveCalculator(this->_game.getPlayer(opponentNumber));
            break;
        default:
            this->_bmc = new RandomMoveCalculator(this->_game.getPlayer(opponentNumber));
            break;
    }
}


void Trainer::train(int amountGames = 1000) {
    this->_networkAgent->load();
    this->_networkAgent->setMode(NetworkMode::Training);

    this->_agent = new QLearningAgent(this->_game.getPlayer(this->_playerNumber), this->_networkAgent);
    Debug::printLine("Start training...");

    int playerOneWinnings = 0;
    int playerTwoWinnings = 0;
    
    
    for(int i = 0; i < amountGames; ++i) {
        int playerTurn = rand() % 2 + 1;
        bool gameIsRunning = true;
        while (gameIsRunning) {
            Player player = this->_game.getPlayer(playerTurn);
            int move = -1;
            Grid state(this->_game.CurrentMap);
            int reward = 0;
            if (playerTurn == 1) {
                // qLearning
                move = this->_agent->chooseAction(this->_game.CurrentMap).Move;
                Game::setStone(player, move, this->_game.CurrentMap);
            } else {
                // Opponent
                move = this->_bmc->getBestMove(this->_game.CurrentMap).Move;
                this->_game.setStone(player, move, this->_game.CurrentMap);
            }
            Grid newState(this->_game.CurrentMap);

            // check if game is over
            if(Game::checkLine(4, this->_game.CurrentMap, player)) {
                if(Debug::getFlag()) {
                    std::cout << "Player " << (int)player.Id << " has won" << std::endl;
                }
                // player has won
                if (playerTurn == 1) {
                    reward = 1;
                    playerOneWinnings++;
                } else {
                    reward = -1;
                    playerTwoWinnings++;
                }
                gameIsRunning = false;
            } else {
                // check for draw
                if (Game::getPossibleMoves(player, this->_game.CurrentMap).size() == 0) {
                    // draw
                    reward = 0;
                    gameIsRunning = false;
                    Debug::printLine("Draw");
                }
            }
            if (playerTurn == 1) {
                this->_networkAgent->addMemory(1, state, move, newState, reward, !gameIsRunning);
            }
            
            playerTurn = (playerTurn % 2) + 1;
            if(Debug::getFlag()) {
                std::cout << this->_game.CurrentMap << std::endl;
            }
        }
        this->_networkAgent->optimize();
    }
    if(true) {
        std::cout << "Player 1 winnings: " << playerOneWinnings << std::endl;
        std::cout << "Player 2 winnings: " << playerTwoWinnings << std::endl;
    } 
    this->_networkAgent->save();
}

Trainer::~Trainer() {
    delete this->_agent;
    delete this->_bmc;
}